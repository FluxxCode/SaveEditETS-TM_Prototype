#include "tmpch.h"
#include "Application.h"

int main(int argc, char* argv[])
{
    Utility::AddLog("----------- Application -----------");

    bool delLog = true;
    for (int i = 0; i < argc; i++)
    {
        std::string str = argv[i];

        Utility::AddLog("[Main] Argument: " + str);

        //Check if app should delete log file
        if (str.find("dellog:false") != std::string::npos)
            delLog = false;
    }

    //Start app with deleting the log file
    Application app(delLog);
    app.Run();

    return 0;
}