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
	"无法打开文件:%s\n该文件可能不存在",
	"获取文件 %s 大小失败！",
	"内存不足！\n无法为文件 %s分配足够的空间",
	"读取文件 %s 失败！",
	"文件指针移动失败！",
	"数据写入文件 %s 失败！",
	"无效的FileProviderDLL",
	"密码错误或者无效的数据包文件 %s，或没有与之相关联的服务组件",
	"缓冲区数据保存失败！您的磁盘空间可能不足",
	"无法找到合适的解码组件！请向文件提供者索取需要的解码组件",
	"数据解码失败！无法还原数据"
};
