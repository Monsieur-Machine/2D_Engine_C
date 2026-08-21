#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "object.h"
#include "utils.h"
#include "errors.h"

typedef enum build_function_t {
    BUILD_OBJECT,
    BUILD_ASSET_FILE
} build_function_t;

void print_usage(void)
{
    puts("\033[33musage\033[m:");
    puts("\tasset_builder output_object_file -c input_file");
    puts("\tasset_builder output_asset_file input_object_file [input_object_file2 ...]");
}

int main(int argc, char *argv[])
{
    // Parse args
    if(argc < 3)
    {
        print_usage();
        return EXIT_FAILURE;
    }

    const build_function_t build_function = strcmp(argv[2], "-c") ? BUILD_ASSET_FILE : BUILD_OBJECT;

    const char *output_file_path = argv[1];
    FILE *output_file = fopen(output_file_path, "wb");

    if(!output_file)
    {
        error_printf("Failed to open output file : %d", errno);
        return EXIT_FAILURE;
    }

    if(build_function == BUILD_OBJECT)
    {
        if(argc != 4)
        {
            print_usage();
            return EXIT_FAILURE;
        }

        const char *input_file_path = argv[3];

        if(build_object(input_file_path, output_file))
        {
            if(fclose(output_file))
                error_printf("Failed to close output file : %d", errno);

            return_failure_int;
        }
    }
    else if(build_function == BUILD_ASSET_FILE)
    {
        uint32_t header_size = 0;

        const int nb_input_object_files = argc - 2;

        if(file_write(&nb_input_object_files, sizeof(nb_input_object_files), 1, output_file))
        {
            if(fclose(output_file))
                error_printf("Failed to close output file : %d", errno);

            return_failure_int;
        }
        header_size += sizeof(nb_input_object_files);

        FILE *input_object_files[nb_input_object_files];

        uint32_t input_object_files_sizes[nb_input_object_files];
        uint32_t input_object_files_offsets[nb_input_object_files];

        for(int i = 0; i < nb_input_object_files; i++)
        {
            const char *input_object_file_path = argv[i + 2];

            input_object_files[i] = fopen(input_object_file_path, "rb");

            if(!input_object_files[i])
            {
                error_printf("Failed to open input object file : %d", errno);

                for(; i >= 0; i--)
                    if(fclose(input_object_files[i]))
                        error_printf("Failed to close input object file : %d", errno);

                if(fclose(output_file))
                    error_printf("Failed to close output file : %d", errno);

                return EXIT_FAILURE;
            }

            if(file_read(&input_object_files_sizes[i], sizeof(*input_object_files_sizes), 1, input_object_files[i]))
            {
                for(; i >= 0; i--)
                    if(fclose(input_object_files[i]))
                        error_printf("Failed to close input object file : %d", errno);

                if(fclose(output_file))
                    error_printf("Failed to close output file : %d", errno);

                return_failure_int
            }

            const char *input_object_file_slash = strrchr(input_object_file_path, '/');
            const char *input_object_file_name = input_object_file_slash ? input_object_file_slash + 1 : input_object_file_path;
            const char *input_object_file_ext = strchr(input_object_file_path, '.');
            uint32_t input_object_file_base_name_length = strlen(input_object_file_name) - strlen(input_object_file_ext) + 1;

            header_size += input_object_file_base_name_length * sizeof(*input_object_file_name);
            header_size += sizeof(*input_object_files_offsets);
        }

        input_object_files_offsets[0] = header_size;

        for(int i = 0; i < nb_input_object_files; i++)
        {
            if(i) input_object_files_offsets[i] = input_object_files_offsets[i - 1] + input_object_files_sizes[i - 1];

            char *input_object_file_path = argv[i + 2];
            const char *input_object_file_slash = strrchr(input_object_file_path, '/');
            const char *input_object_file_name = input_object_file_slash ? input_object_file_slash + 1 : input_object_file_path;
            char *input_object_file_ext = strchr(input_object_file_path, '.');
            input_object_file_ext[0] = '\00';
            uint32_t input_object_file_base_name_length = strlen(input_object_file_name) - strlen(input_object_file_ext) + 1;

            if(file_write(input_object_file_name, sizeof(*input_object_file_name), input_object_file_base_name_length, output_file))
            {
                for(i = 0; i < nb_input_object_files; i++)
                    if(fclose(input_object_files[i]))
                        error_printf("Failed to close input object file : %d", errno);

                if(fclose(output_file))
                    error_printf("Failed to close output file : %d", errno);

                return_failure_int;
            }

            if(file_write(&input_object_files_offsets[i], sizeof(*input_object_files_offsets), 1, output_file))
            {
                for(i = 0; i < nb_input_object_files; i++)
                    if(fclose(input_object_files[i]))
                        error_printf("Failed to close input object file : %d", errno);

                if(fclose(output_file))
                    error_printf("Failed to close output file : %d", errno);

                return_failure_int;
            }
        }

        for(int i = 0; i < nb_input_object_files; i++)
        {
            char buffer[input_object_files_sizes[i]];

            if(file_read(buffer, sizeof(*buffer), input_object_files_sizes[i], input_object_files[i]))
            {
                for(i = 0; i < nb_input_object_files; i++)
                    if(fclose(input_object_files[i]))
                        error_printf("Failed to close input object file : %d", errno);

                if(fclose(output_file))
                    error_printf("Failed to close output file : %d", errno);

                return_failure_int;
            }

            if(file_write(buffer, sizeof(*buffer), input_object_files_sizes[i], output_file))
            {
                for(i = 0; i < nb_input_object_files; i++)
                    if(fclose(input_object_files[i]))
                        error_printf("Failed to close input object file : %d", errno);

                if(fclose(output_file))
                    error_printf("Failed to close output file : %d", errno);

                return_failure_int;
            }
        }

        for(int i = 0; i < nb_input_object_files; i++)
        {
            if(fclose(input_object_files[i]))
            {
                error_printf("Failed to close input object file : %d", errno);

                for(i++; i < nb_input_object_files; i++)
                    if(fclose(input_object_files[i]))
                        error_printf("Failed to close input object file : %d", errno);

                if(fclose(output_file))
                    error_printf("Failed to close output file : %d", errno);

                return EXIT_FAILURE;
            }
        }
    }

    if(fclose(output_file))
    {
        error_printf("Failed to close output file : %d", errno);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
