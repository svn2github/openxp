#define SAFE_DELETE(A)	if( (A) != NULL ) { delete [] (A); (A) = NULL; }
#define SAFE_RELEASE(A)	if( (A) != NULL ) { (A) -> Release(); (A) = NULL; }

const DWORD	JSPKFileFlag	= 0x4B50534A;

#define FILE_PROVIDER_MASK	0x4650454C
#define CODEC_PROVIDER_MASK	0x45444650

#define MAX_PWD		255

enum ENCODE_TYPE{ENC_NONE,ENC_RLE,ENC_LZW};

enum FILE_ERR_CODE{
	FERR_NONE,
	FERR_OPENFAILED,
	FERR_GETSIZEFAILED,
	FERR_MEMLOW,
	FERR_READFAILED,
	FERR_SEEKFAILED,
	FERR_SAVEFAILED,
	FERR_INVALIDPRO,
	FERR_INVALIDPAK,
	FERR_SAVEBUFFAILED,
	FERR_NODECODER,
	FERR_DECODEFAILED
};
const char FILE_ERR_MSG[][60]=
{
	"\0",
	"�޷����ļ�:%s\n���ļ����ܲ�����",
	"��ȡ�ļ� %s ��Сʧ�ܣ�",
	"�ڴ治�㣡\n�޷�Ϊ�ļ� %s�����㹻�Ŀռ�",
	"��ȡ�ļ� %s ʧ�ܣ�",
	"�ļ�ָ���ƶ�ʧ�ܣ�",
	"����д���ļ� %s ʧ�ܣ�",
	"��Ч��FileProviderDLL",
	"������������Ч�����ݰ��ļ� %s����û����֮������ķ������",
	"���������ݱ���ʧ�ܣ����Ĵ��̿ռ���ܲ���",
	"�޷��ҵ����ʵĽ�������������ļ��ṩ����ȡ��Ҫ�Ľ������",
	"���ݽ���ʧ�ܣ��޷���ԭ����"
};
