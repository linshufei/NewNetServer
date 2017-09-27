#include <stdint.h>

enum HEADERTYPE
{
    KRJPEG = 0x0f0f0f0f,
    KRH264
};

//�ļ�ͷ�ṹ��
typedef struct SImageHeader
{
    uint32_t headerType;        //�������ͱ�־λ
    uint32_t width;             //ͼƬ�Ŀ��
    uint32_t height;            //ͼƬ�ĸ߶�
    uint32_t serverType;        //����ķ�������
    uint32_t dataOffset;        //ͼƬ����ͷ��λ��
    uint32_t dataSize;          //ͼƬ��С
    uint32_t headerCheckCode;   //У����
} SImageHeader;