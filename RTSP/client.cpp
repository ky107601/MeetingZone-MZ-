#include <iostream>
#include <thread>
#include <cstdlib>
#include <csignal>
#include <chrono>
#include <string>
#include <opencv2/opencv.hpp>

extern "C" {
    #include <ifaddrs.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>

    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/avutil.h>
    #include <libavutil/imgutils.h>
    #include <libavdevice/avdevice.h>
    #include <libswscale/swscale.h>
    #include <libavutil/time.h>
}

std::string get_ip_addr(){
    struct ifaddrs *ifap = nullptr;
    struct ifaddrs *i = nullptr;
    void *src;
    std::string ip_address = "";

    if(getifaddrs(&ifap)==0){ //creates a linked list of structures describing the network interfaces of the local system
        for(i = ifap; i != nullptr; i=i->ifa_next){
            if(i->ifa_addr == nullptr)
                continue;
            
            if(i->ifa_addr->sa_family == AF_INET){
                char tmp[INET_ADDRSTRLEN];
                src = &((struct sockaddr_in *)i->ifa_addr)->sin_addr;
                inet_ntop(AF_INET, src, tmp, INET_ADDRSTRLEN); // converts the network address structure to character string
                if(strcmp(i->ifa_name, "lo")!=0){ // 127.0.0.1 제외
                    ip_address = tmp;
                    break;
                }
            }
        }
    }

    if (ifap != nullptr) {
        freeifaddrs(ifap);
    }

    return ip_address;
}

// Function to start MediaMTX server
void startMediaMTX() {
    std::cout << "Starting MediaMTX server..." << std::endl;

    // Command to start MediaMTX (adjust path as needed)
    int result = std::system("./mediamtx &");
    if (result != 0) {
        std::cerr << "Failed to start MediaMTX. Please ensure it's installed and accessible." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Wait for the server to initialize (optional, adjust timing)
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "MediaMTX server started successfully." << std::endl;
}

// Function to stop MediaMTX server
void stopMediaMTX() {
    std::cout << "Stopping MediaMTX server..." << std::endl;

    // Kill the MediaMTX process (use pkill or PID tracking)
    int result = std::system("pkill mediamtx");
    if (result != 0) {
        std::cerr << "Failed to stop MediaMTX. Please check manually." << std::endl;
    } else {
        std::cout << "MediaMTX server stopped successfully." << std::endl;
    }
}

void rtsp_streaming(const std::string& rtsp_url) {
    // Initialize FFmpeg
    avformat_network_init();

    // RTSP Output Context
    AVFormatContext* output_ctx = nullptr;
    AVStream* video_stream = nullptr;

    // Codec and codec context
    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        std::cerr << "H.264 codec not found!" << std::endl;
        return;
    }

    // Video codec context
    AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
        std::cerr << "Failed to allocate codec context!" << std::endl;
        return;
    }

    // Configure codec parameters
    codec_ctx->codec_id = AV_CODEC_ID_H264;
    codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    codec_ctx->bit_rate = 400000;
    codec_ctx->width = 640;
    codec_ctx->height = 480;
    codec_ctx->time_base = AVRational{1, 25};
    codec_ctx->gop_size = 10;
    codec_ctx->max_b_frames = 1;
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;

    if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
        std::cerr << "Failed to open codec!" << std::endl;
        avcodec_free_context(&codec_ctx);
        return;
    }

    // Allocate frame and buffer
    AVFrame* frame = av_frame_alloc();
    if (!frame) {
        std::cerr << "Failed to allocate frame!" << std::endl;
        avcodec_free_context(&codec_ctx);
        return;
    }
    frame->format = codec_ctx->pix_fmt;
    frame->width = codec_ctx->width;
    frame->height = codec_ctx->height;

    int buffer_size = av_image_get_buffer_size(codec_ctx->pix_fmt, codec_ctx->width, codec_ctx->height, 1);
    uint8_t* buffer = (uint8_t*)av_malloc(buffer_size);
    av_image_fill_arrays(frame->data, frame->linesize, buffer, codec_ctx->pix_fmt, codec_ctx->width, codec_ctx->height, 1);

    // Open output context
    if (avformat_alloc_output_context2(&output_ctx, nullptr, "rtsp", rtsp_url.c_str()) < 0) {
        std::cerr << "Failed to create output context!" << std::endl;
        av_frame_free(&frame);
        av_free(buffer);
        avcodec_free_context(&codec_ctx);
        return;
    }

    video_stream = avformat_new_stream(output_ctx, nullptr);
    if (!video_stream) {
        std::cerr << "Failed to create video stream!" << std::endl;
        avformat_free_context(output_ctx);
        av_frame_free(&frame);
        av_free(buffer);
        avcodec_free_context(&codec_ctx);
        return;
    }

    if (avcodec_parameters_from_context(video_stream->codecpar, codec_ctx) < 0) {
        std::cerr << "Failed to copy codec parameters!" << std::endl;
        avformat_free_context(output_ctx);
        av_frame_free(&frame);
        av_free(buffer);
        avcodec_free_context(&codec_ctx);
        return;
    }

    video_stream->time_base = codec_ctx->time_base;

    if (!(output_ctx->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&output_ctx->pb, rtsp_url.c_str(), AVIO_FLAG_WRITE) < 0) {
            std::cerr << "Failed to open RTSP output!" << std::endl;
            avformat_free_context(output_ctx);
            av_frame_free(&frame);
            av_free(buffer);
            avcodec_free_context(&codec_ctx);
            return;
        }
    }

    if (avformat_write_header(output_ctx, nullptr) < 0) {
        std::cerr << "Failed to write RTSP header!" << std::endl;
        avformat_free_context(output_ctx);
        av_frame_free(&frame);
        av_free(buffer);
        avcodec_free_context(&codec_ctx);
        return;
    }

    std::cout << "RTSP streaming started on " << rtsp_url << std::endl;

    // OpenCV camera capture
    cv::VideoCapture cap("libcamerasrc camera-name=/base/axi/pcie@120000/rp1/i2c@88000/ov5647@36 \
    ! video/x-raw,width=640,height=480,framerate=25/1,format=BGR ! appsink", cv::CAP_GSTREAMER);  // Adjust the camera index as needed
    if (!cap.isOpened()) {
        std::cerr << "Failed to open camera!" << std::endl;
        avformat_free_context(output_ctx);
        av_frame_free(&frame);
        av_free(buffer);
        avcodec_free_context(&codec_ctx);
        return;
    }

    cv::Mat image;
    SwsContext* sws_ctx = sws_getContext(
        codec_ctx->width, codec_ctx->height, AV_PIX_FMT_BGR24,
        codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt,
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    int frame_count = 0;
    int64_t pts = 0;

    while (cap.read(image)) {
        // Convert to YUV format
        const uint8_t* data[1] = {image.data};
        int linesize[1] = {static_cast<int>(image.step[0])};
        sws_scale(sws_ctx, data, linesize, 0, codec_ctx->height, frame->data, frame->linesize);

        // 타임스탬프 계산
        frame->pts = pts;
        pts += codec_ctx->time_base.den / codec_ctx->time_base.num;  // 프레임 간 일정 간격 유지

        AVPacket *pkt = av_packet_alloc();
        int ret = avcodec_send_frame(codec_ctx, frame);
        if (ret >= 0) {
            while (ret >= 0) {
                ret = avcodec_receive_packet(codec_ctx, pkt);
                if (ret == 0) {
                    pkt->stream_index = video_stream->index;
                    pkt->pts = pkt->dts = frame_count++;

                    std::cout << "PTS: " << pkt->pts << std::endl;

                    // 패킷 쓰기 전 추가 검사
                    if (pkt->pts >= 0) {
                        av_packet_rescale_ts(pkt, codec_ctx->time_base, video_stream->time_base);
                        ret = av_interleaved_write_frame(output_ctx, pkt);
                        if (ret < 0) {
                            char errbuf[AV_ERROR_MAX_STRING_SIZE];
                            av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
                            std::cerr << "Error writing frame: " << errbuf << std::endl;
                        }
                    }
                    
                    av_packet_unref(pkt);
                } else if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else {
                    char errbuf[AV_ERROR_MAX_STRING_SIZE];
                    av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
                    std::cerr << "Error encoding frame: " << errbuf << std::endl;
                    break;
                }
            }
        }

        av_packet_free(&pkt);
    }
    // Cleanup
    av_write_trailer(output_ctx);
    avcodec_free_context(&codec_ctx);
    avformat_free_context(output_ctx);
    av_frame_free(&frame);
    av_free(buffer);
    sws_freeContext(sws_ctx);
    cap.release();

    std::cout << "RTSP streaming stopped." << std::endl;
}

int main() {
    const std::string rtsp_url = "rtsps://" + get_ip_addr() + ":8322/camera";

    // Register signal handler to clean up resources
    std::signal(SIGINT, [](int) {
        stopMediaMTX();
        exit(EXIT_SUCCESS);
    });

    // Start MediaMTX server
    startMediaMTX();

    // Start RTSP streaming in a separate thread
    std::thread streaming_thread(rtsp_streaming, rtsp_url);

    // Wait for the streaming thread to complete
    streaming_thread.join();

    // Stop MediaMTX server before exiting
    stopMediaMTX();

    return 0;
}