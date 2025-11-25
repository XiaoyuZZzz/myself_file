#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ==================== 文件操作职责 ====================

// 文件读取器 - 只负责文件的读取操作
typedef struct
{
    char *content;
    size_t length;
} FileReader;

FileReader *create_file_reader()
{
    return (FileReader *)malloc(sizeof(FileReader));
}

int read_file(FileReader *reader, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("无法打开文件: %s\n", filename);
        return 0;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    reader->length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 分配内存并读取内容
    reader->content = (char *)malloc(reader->length + 1);
    if (!reader->content)
    {
        fclose(file);
        return 0;
    }

    fread(reader->content, 1, reader->length, file);
    reader->content[reader->length] = '\0';

    fclose(file);
    return 1;
}

void destroy_file_reader(FileReader *reader)
{
    if (reader)
    {
        free(reader->content);
        free(reader);
    }
}

// ==================== 文本处理职责 ====================

// 文本处理器 - 只负责文本内容的处理
typedef struct
{
    int word_count;
    int char_count;
    int line_count;
} TextProcessor;

TextProcessor *create_text_processor()
{
    return (TextProcessor *)malloc(sizeof(TextProcessor));
}

void process_text(TextProcessor *processor, const char *text)
{
    processor->word_count = 0;
    processor->char_count = 0;
    processor->line_count = 0;

    if (!text)
        return;

    int in_word = 0;
    const char *ptr = text;

    while (*ptr)
    {
        processor->char_count++;

        if (*ptr == '\n')
        {
            processor->line_count++;
        }

        if (isspace(*ptr))
        {
            in_word = 0;
        }
        else if (!in_word)
        {
            processor->word_count++;
            in_word = 1;
        }

        ptr++;
    }

    // 如果文本不以换行符结束，也要计为一行
    if (processor->char_count > 0)
    {
        processor->line_count++;
    }
}

void print_statistics(const TextProcessor *processor)
{
    printf("文本统计信息:\n");
    printf("  字符数: %d\n", processor->char_count);
    printf("  单词数: %d\n", processor->word_count);
    printf("  行数: %d\n", processor->line_count);
}

void destroy_text_processor(TextProcessor *processor)
{
    free(processor);
}

// ==================== 数据保存职责 ====================

// 数据保存器 - 只负责数据的保存操作
typedef struct
{
    const char *output_filename;
} DataSaver;

DataSaver *create_data_saver(const char *filename)
{
    DataSaver *saver = (DataSaver *)malloc(sizeof(DataSaver));
    saver->output_filename = strdup(filename);
    return saver;
}

int save_statistics(DataSaver *saver, const TextProcessor *processor)
{
    FILE *file = fopen(saver->output_filename, "w");
    if (!file)
    {
        printf("无法创建输出文件: %s\n", saver->output_filename);
        return 0;
    }

    fprintf(file, "文本统计报告\n");
    fprintf(file, "=============\n");
    fprintf(file, "字符数: %d\n", processor->char_count);
    fprintf(file, "单词数: %d\n", processor->word_count);
    fprintf(file, "行数: %d\n", processor->line_count);

    fclose(file);
    printf("统计结果已保存到: %s\n", saver->output_filename);
    return 1;
}

void destroy_data_saver(DataSaver *saver)
{
    if (saver)
    {
        free((void *)saver->output_filename);
        free(saver);
    }
}

// ==================== 主程序 - 协调各个职责 ====================

int main()
{
    const char *input_file = "input.txt";
    const char *output_file = "statistics.txt";

    // 1. 文件读取职责
    FileReader *file_reader = create_file_reader();
    if (!read_file(file_reader, input_file))
    {
        printf("文件读取失败\n");
        destroy_file_reader(file_reader);
        return 1;
    }

    printf("成功读取文件: %s\n", input_file);
    printf("文件内容:\n%s\n", file_reader->content);

    // 2. 文本处理职责
    TextProcessor *text_processor = create_text_processor();
    process_text(text_processor, file_reader->content);
    print_statistics(text_processor);

    // 3. 数据保存职责
    DataSaver *data_saver = create_data_saver(output_file);
    save_statistics(data_saver, text_processor);

    // 清理资源
    destroy_file_reader(file_reader);
    destroy_text_processor(text_processor);
    destroy_data_saver(data_saver);

    return 0;
}