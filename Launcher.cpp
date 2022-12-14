#include <io.h>
#include <string.h>
#include <ShlObj.h>
#include <chrono>
#include <thread>

#include "ini.h"
using namespace mINI;
using namespace std;
using namespace this_thread; // sleep_for, sleep_until
using namespace chrono; // nanoseconds, system_clock, seconds

bool file_exists(const char* filename) {
	return (_access(filename, 0) == 0);
}

int main() 
{
	if (!IsUserAnAdmin()) {
		printf("启动器需要以管理员方式运行!\n");
		system("pause");
		return 0;
	}

	if (!(file_exists((char*)"launcher.ini") && file_exists((char*)"netif.bat"))) {
		printf("启动器缺少必要的文件!\n");
		system("pause");
		return 0;
	}

	INIFile launcher("launcher.ini");

	INIStructure launcher_structure;
	
	launcher.read(launcher_structure);

	string& enable_launch_chunithm = launcher_structure["Launcher"]["enableLaunchChunithm"];
	string& pause_after_finish = launcher_structure["Launcher"]["pauseAfterFinish"];
	string& shadowtenpo_path = launcher_structure["launcher"]["shadowtenpoPath"];
	string& chunithm_path = launcher_structure["launcher"]["chunithmPath"];
	
	if (!file_exists((shadowtenpo_path + "\\start.bat").data())) {
		printf("Shadowtenpo 缺少必要的文件!\n");
		system("pause");
		return 0;
	}

	if (!(file_exists((chunithm_path + "\\segatools.ini").data()) && file_exists((chunithm_path + "\\start.bat").data()))) {
		printf("Chunithm HDD 缺少必要的文件!\n");
		system("pause");
		return 0;
	}

	if (file_exists((shadowtenpo_path + "\\segatools-override.ini").data())) {
		system(("del /f /q " + shadowtenpo_path + "\\segatools-override.ini").data());
	}
	
	system("netif.bat");
	system("cls");
	system(("cd /d " + shadowtenpo_path + " && start " + shadowtenpo_path + "\\start.bat").data());
	
	for (int i = 1; i < 11; i++) {
		printf("正在等待 segatools-override.ini 生成, 第 %d 次尝试\n", i);
		if (file_exists((shadowtenpo_path + "\\segatools-override.ini").data())) {
			break;
		}
		else {
			sleep_for(seconds(3));
		}
	}
	
	if (!file_exists((shadowtenpo_path + "\\segatools-override.ini").data())) {
		printf("Shadowtenpo 启动失败!\n");
		system("pause");
		return 0;
	}

	INIFile segatools_override((shadowtenpo_path + "\\segatools-override.ini").data());
	
	INIStructure segatools_override_structure;
	
	segatools_override.read(segatools_override_structure);
	
	string& segatools_override_enable = segatools_override_structure["netenv"]["enable"];
	string& segatools_override_addrsuffix = segatools_override_structure["netenv"]["addrSuffix"];
	string& segatools_override_subnet = segatools_override_structure["keychip"]["subnet"];

	INIFile segatools((chunithm_path + "\\segatools.ini").data());
	
	INIStructure segatools_structure;

	segatools.read(segatools_structure);

	segatools_structure["netenv"]["enable"] = segatools_override_enable;
	segatools_structure["netenv"]["addrSuffix"] = segatools_override_addrsuffix;
	segatools_structure["keychip"]["subnet"] = segatools_override_subnet;

	segatools.write(segatools_structure);

	system(("del /f /q " + shadowtenpo_path + "\\segatools-override.ini").data());

	if (enable_launch_chunithm == "1") {
		system(("cd /d " + chunithm_path + " && start " + chunithm_path + "\\start.bat").data());
		printf("你好，我是中二企鹅，游戏已启动! \n");
	}
	else {
		printf("启动器已完成!\n");
	}

	if (pause_after_finish == "1") {
		system("pause");
	}

	return 0;
}
