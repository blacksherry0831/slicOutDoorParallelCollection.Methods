/*----------------------------------------------------------------*/
#include "opencv_stl.h"
#include "cpp_def.h"
/*----------------------------------------------------------------*/
#include <SGV/SGV_Method.h>
#include <SGV/SGV_CFG_DBG.h>
#include <SGV/SGV_CFG_ARITHMETIC.h>
/*----------------------------------------------------------------*/
#include <SGVcode/cui_GeneralImgProcess.h>
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int EXEC_RESULT = 0;

	QApplication a(argc, argv);
	{
		std::string file_name = "X:\\ImageDataBase\\1920.1080\\r.jpeg";
		
		SGV_Method::METHOD_FILE(file_name);
	
	}
	
	return  0;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/