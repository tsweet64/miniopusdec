#include<opus/opusfile.h>
#include<iostream>
#include<fstream>

const int OPUS_NUM_SAMPLES = 17280;

int main(int argc, char** argv) {
    // Get filename from command line args
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " filename.opus outfile\n";
        return 1;
    }
    char* filename = argv[1];
    char* outfile = argv[2];

    //open the opus file
    int openstatus = 0;
    OggOpusFile* opusfile = op_open_file(filename, &openstatus);
    if (openstatus) {
        std::cout << "There was an error opening the file.\n";
        return 1;
    }
    std::cout << "The file was opened successfully\n";

    //get channel count
    int channel_cnt = op_channel_count(opusfile, -1);
    std::cout << "Channel count: " << channel_cnt << std::endl;

    //get time
    long sample_cnt = op_pcm_total(opusfile, -1);
    double time_s = static_cast<double>(sample_cnt) / 48000.0;
    std::cout << "Duration (s): " << time_s << std::endl;

    //get bitrate
    opus_int32 bitrate = op_bitrate(opusfile, -1);
    std::cout << "Bitrate: " << static_cast<double>(bitrate)/1000.0/channel_cnt << " kb/s\n";

    //get tags
    std::cout << "\n ---Begin tags---\n";
    const OpusTags* tags = op_tags(opusfile, -1);
    std::cout << "Vendor/encoder: " << tags->vendor << std::endl;
    for (int i = 0; i < sizeof(tags->user_comments); i++) {
        std::cout << tags->user_comments[i] << std::endl;
    }

    //Initialize an output file stream for the decoded pcm
    std::fstream outpcm = std::fstream(outfile, std::ios::out | std::ios::app | std::ios::binary);

    // Use opus decoder. Decodes ~120ms at a time. For simplicity we only implement stereo.
    opus_int16* dec_buffer= new opus_int16[OPUS_NUM_SAMPLES*2];
    int numread;
    long itercount = 0; // Keep an iterator so we can print the bitrate every n frames
    while(true) {
        itercount++;
        int numread = op_read_stereo(opusfile, dec_buffer, OPUS_NUM_SAMPLES*2);
        if (numread == -3) {
            std::cout << "Potentially recoverable error detected.\n";
            continue;
        } else if (numread < 0) {
            std::cout << "There was an error decoding the stream. " << numread << std::endl;
            break;
        } else if (numread == 0) {
            std::cout << "End of file\n";
            break;
        }
        //std::cout << "read count: " << numread << " bytepos: " << op_raw_tell(opusfile) << std::endl;
        if (itercount % 10 == 0) {
            std::cout << "Current bitrate: " << static_cast<double>(op_bitrate_instant(opusfile))/1000.0 << " kb/s\n";
        }
        // write a block of raw pcm to outfile
        outpcm.write((char*)dec_buffer, numread*channel_cnt*2);
    }
    outpcm.close();
}
