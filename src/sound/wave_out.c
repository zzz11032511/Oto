#include "wave_out.h"

#include <stdio.h>
#include <stdint.h>

#include "../utils/util.h"

void mono_wave_write(MONO_PCM *pcm, str_t filename)
{
    int8_t riff_chunk_ID[4];
    riff_chunk_ID[0] = 'R';
    riff_chunk_ID[1] = 'I';
    riff_chunk_ID[2] = 'F';
    riff_chunk_ID[3] = 'F';

    int32_t riff_chunk_size = 36 + pcm->length * 2;
    
    int8_t riff_form_type[4];
    riff_form_type[0] = 'W';
    riff_form_type[1] = 'A';
    riff_form_type[2] = 'V';
    riff_form_type[3] = 'E';

    int8_t fmt_chunk_ID[4];
    fmt_chunk_ID[0] = 'f';
    fmt_chunk_ID[1] = 'm';
    fmt_chunk_ID[2] = 't';
    fmt_chunk_ID[3] = ' ';

    int32_t fmt_chunk_size = 16;

    int16_t fmt_wave_format_type = 1;  // PCM
    int16_t fmt_channel = 1;  // MONO
    int32_t fmt_samples_per_sec = pcm->sampling_freq;  // sampling freqency
    int32_t fmt_bytes_per_sec = pcm->sampling_freq * pcm->bits / 8;
    int16_t fmt_block_size = pcm->bits / 8;
    int16_t fmt_bits_per_sample = pcm->bits;  // quantization bit length

    int8_t data_chunk_ID[4];
    data_chunk_ID[0] = 'd';
    data_chunk_ID[1] = 'a';
    data_chunk_ID[2] = 't';
    data_chunk_ID[3] = 'a';
    
    int32_t data_chunk_size = pcm->length * 2;
    
    FILE *fp = fopen(filename, "wb");
    
    fwrite(riff_chunk_ID, 1, 4, fp);
    fwrite(&riff_chunk_size, 4, 1, fp);
    fwrite(riff_form_type, 1, 4, fp);
    fwrite(fmt_chunk_ID, 1, 4, fp);
    fwrite(&fmt_chunk_size, 4, 1, fp);
    fwrite(&fmt_wave_format_type, 2, 1, fp);
    fwrite(&fmt_channel, 2, 1, fp);
    fwrite(&fmt_samples_per_sec, 4, 1, fp);
    fwrite(&fmt_bytes_per_sec, 4, 1, fp);
    fwrite(&fmt_block_size, 2, 1, fp);
    fwrite(&fmt_bits_per_sample, 2, 1, fp);
    fwrite(data_chunk_ID, 1, 4, fp);
    fwrite(&data_chunk_size, 4, 1, fp);
    
    double s = 0;
    for(int32_t i = 0; i < pcm->length; i++){
        s = (pcm->s[i] + 1.0) / 2.0 * 65536.0;
        
        if (s > 65535.0) {
            s = 65535.0;
        } else if (s < 0.0) {
            s = 0.0;
        }
        
        int16_t data = (short)(s + 0.5) - 32768;
        fwrite(&data, 2, 1, fp);
    }
    
    fclose(fp);
}