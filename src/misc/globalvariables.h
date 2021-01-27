#ifndef GLOBALVARIABLES
#define GLOBALVARIABLES

#include <iostream>

#include <string>

///Data Directory File Path

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)

//DATADIR macro used by automake for path to data files
//if DATADIR macro is not defined, define it as data
#ifndef DATADIR
    #define DATADIR "../data"
#endif

#define DATADIR_NAME STR(DATADIR)

extern std::string DATADIR_STR;

//Place Directory File Path
extern std::string PLACEDIR_STR;

//Buttons Directory file path
extern std::string BUTTONSDIR_STR;

//weapons directory file path
extern std::string WEAPONS_IMAGE_DIR_STR;

//Menu directory file path
extern std::string MENU_IMAGE_DIR_STR;

 //tile dimensions 
extern std::int16_t globalTileWidth;
extern std::int16_t globalTileHeight;


#endif // GLOBALVARIABLES

