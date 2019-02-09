#include <iostream>
#include <mirror_utils_log.h>
#include <cmdline.h>

int main(int argc, char** argv)
{
	cmdline::parser cmd;
	cmd.add("filename", 'f', "filename");
	cmd.parse_check(argc, argv);

	if(cmd.exist("filename"))
	{
		std::string filename = cmd.get<std::string>("filename");
		LOG_DEBUG("filename : " << filename);
	}

	return 0;
}
