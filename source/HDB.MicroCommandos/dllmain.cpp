#include "stdafx.h"

bool bHACKMEfixes;

void Init()
{
	
	/* TODO	
	complete general gameplay (livestream)

    BUGS:
    missing main menu logo on new resoliutions
	*/

    std::string TakeoverMD = "Micro Commandos HDBeefup | Compiled at " __DATE__ ", " __TIME__ " |";
	
    CIniReader iniReader("mchd.ini");
    int ResX = iniReader.ReadInteger("MAIN", "ResX", 0);
    int ResY = iniReader.ReadInteger("MAIN", "ResY", 0);
    bHACKMEfixes = iniReader.ReadInteger("Patchworks", "HACKMEfixes", 1);

    if (!ResX || !ResY)
        std::tie(ResX, ResY) = GetDesktopRes();

/* GAME EXE SCREEN RES */

/* GUI FILES BEEFUP */
CIniReader iniWriter("..\\Pannels\\menuprincipal.ini");
iniWriter.WriteInteger("MENUPRINCIPAL", "WIDTH", ResX );
iniWriter.WriteInteger("MENUPRINCIPAL", "HEIGHT", ResY);

iniWriter.WriteInteger("MAINMENU_LOGOMICROROOT", "X", ResX - 610);
iniWriter.WriteInteger("MAINMENU_LOGOMICRO", "X", ResX - 570); //71
iniWriter.WriteInteger("MAINMENU_LOGOMICRO2", "X", ResX - 314);

if (ResY >= 768 && bHACKMEfixes) {  /* HACK ME*/
iniWriter.WriteInteger("MAINMENU_LOGOMICROROOT", "X", -(((ResX * 76) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_LOGOMICRO", "X", -(((ResX * 54) / 100) - ResX) - 4); //71
iniWriter.WriteInteger("MAINMENU_LOGOMICRO2", "X", -(((ResX * 39) / 100) - ResX));

iniWriter.WriteInteger("MAINMENU_PRINCIPALE02", "X", -(((ResX * 68) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_PRINCIPALE03", "X", -(((ResX * 36) / 100) - ResX));

iniWriter.WriteInteger("MAINMENU_PRINCIPALEROOT", "WIDTH", -(((ResX * 36) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_PRINCIPALEROOT", "HEIGHT", -(((ResY * 15) / 100) - ResY));

iniWriter.WriteInteger("MAINMENU_NEWGAME_T", "X", -(((ResX * 60) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_NEWGAME", "X", -(((ResX * 66) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_REPLAY_T", "X", -(((ResX * 76) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_REPLAY", "X", -(((ResX * 83) / 100) - ResX));

iniWriter.WriteInteger("MAINMENU_CONTINUE_T", "X", -(((ResX * 60) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_CONTINUE", "X", -(((ResX * 66) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_LOAD", "X", -(((ResX * 54) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_LOAD_T", "X", -(((ResX * 47) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_SELECTPLAYER_T", "X", -(((ResX * 38) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_SELECTPLAYER", "X", -(((ResX * 44) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_QUIT_T", "X", -(((ResX * 31) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_QUIT", "X", -(((ResX * 37) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_CREDITS_T", "X", -(((ResX * 26) / 100) - ResX));
iniWriter.WriteInteger("MAINMENU_CREDITS", "X", -(((ResX * 29) / 100) - ResX));
}

iniWriter.SetIniPath("Pannels\\menucredits.ini");
iniWriter.WriteInteger("MENUCREDITS", "WIDTH", ResX);
iniWriter.WriteInteger("MENUCREDITS", "HEIGHT", ResY);

iniWriter.SetIniPath("Pannels\\quartdecercle.ini"); // In game Bottom UI
iniWriter.WriteInteger("QUARTDECERCLE", "Y", ResY-156 ); // 444 if 800x600, 26% however makes gap @1680x1050 of 117px (same for rest of UI. selection, objects) but makes scale proper

iniWriter.SetIniPath("Pannels\\bouton_minimap.ini"); // In game TOP UI
iniWriter.WriteInteger("MINIMAP_ROOT", "X", ResX - 80); // 10  720
iniWriter.WriteInteger("NAVIGATION", "X", ResX - 230);  // 29 570

iniWriter.SetIniPath("Pannels\\menuingame.Ini");
iniWriter.WriteInteger("MENUINGAME_BLACK", "WIDTH", ResX);
iniWriter.WriteInteger("MENUINGAME_BLACK", "HEIGHT", ResY);

iniWriter.SetIniPath("Pannels\\oui_non.INI");
iniWriter.WriteInteger("MENUINGAME_BLACK_YESNO", "WIDTH", ResX);
iniWriter.WriteInteger("MENUINGAME_BLACK_YESNO", "HEIGHT", ResY);

iniWriter.WriteInteger("MENU_OUI_NON", "WIDTH", ResX);
iniWriter.WriteInteger("MENU_OUI_NON", "HEIGHT", ResY);

iniWriter.WriteInteger("LIST_MENU_OUI_NON_TEXT", "X", ResX - 485); //315
iniWriter.WriteInteger("LIST_MENU_OUI_NON_TEXT", "Y", ResY - 330); //270
iniWriter.WriteInteger("MENU_OUI_NON_GAUCHE", "X", ResX - 570); //230 MAINLY YES NO BG
iniWriter.WriteInteger("MENU_OUI_NON_GAUCHE", "Y", ResY - 360); //240
iniWriter.WriteInteger("MENU_OUI_NON_DROIT", "X", ResX - 314); //486 UI AND 2ND PART YES NO BG
iniWriter.WriteInteger("MENU_OUI_NON_DROIT", "Y", ResY - 360); //240
iniWriter.WriteInteger("MENU_NON", "X", ResX - 546); //254
iniWriter.WriteInteger("MENU_NON", "Y", ResY - 195); //405
iniWriter.WriteInteger("MENU_OUI", "X", ResX - 342); //458
iniWriter.WriteInteger("MENU_OUI", "Y", ResY - 204); //396 

if (ResY >= 1050 && bHACKMEfixes) { /* HACK ME*/
    iniWriter.WriteInteger("LIST_MENU_OUI_NON_TEXT", "X", -(((ResX * 60.625) / 100) - ResX)); //315
    iniWriter.WriteInteger("LIST_MENU_OUI_NON_TEXT", "Y", -(((ResY * 55) / 100) - ResY) - 14); //270
    iniWriter.WriteInteger("MENU_OUI_NON_GAUCHE", "X", -(((ResX * 60.625) / 100) - ResX) - 80); //230
    iniWriter.WriteInteger("MENU_OUI_NON_GAUCHE", "Y", -(((ResY * 60) / 100) - ResY)); //240
    iniWriter.WriteInteger("MENU_OUI_NON_DROIT", "X", -(((ResX * 60.625) / 100) - ResX) + 176); //486
    iniWriter.WriteInteger("MENU_OUI_NON_DROIT", "Y", -(((ResY * 60) / 100) - ResY)); //240
    iniWriter.WriteInteger("MENU_NON", "X", -(((ResX * 60.625) / 100) - ResX) - 56); //254
    iniWriter.WriteInteger("MENU_NON", "Y", -(((ResY * 32.5) / 100) - ResY) - 123); //405
    iniWriter.WriteInteger("MENU_OUI", "X", -(((ResX * 60.625) / 100) - ResX) + 175 - 27); //458
    iniWriter.WriteInteger("MENU_OUI", "Y", -(((ResY * 34) / 100) - ResY) - 117); //396
}

iniWriter.SetIniPath("Pannels\\BANDEAU.INI");
iniWriter.WriteInteger("cadre_noms", "Y", ResY-143);   // 24 /// name part    
//iniWriter.WriteInteger("cadre_noms", "HEIGHT", -(((ResY * 93) / 100) - ResY));

iniWriter.WriteInteger("bandeau4_fond", "Y", ResY - 112); // 19 /// ? GUESS MAIN BG
//iniWriter.WriteInteger("bandeau4_fond", "HEIGHT", -(((ResY * 74) / 100) - ResY));
iniWriter.WriteInteger("bandeau3_fond", "Y", ResY - 112); // 19 /// ? BG???
//iniWriter.WriteInteger("bandeau3_fond", "HEIGHT", -(((ResY * 74) / 100) - ResY));
iniWriter.WriteInteger("bandeau2_fond", "Y", ResY - 112); // 19 /// ? BG???
//iniWriter.WriteInteger("bandeau2_fond", "HEIGHT", -(((ResY * 74) / 100) - ResY));
iniWriter.WriteInteger("bandeau_fond", "Y", ResY - 112);  // 19 /// bg1 (GRAY)
//iniWriter.WriteInteger("bandeau_fond", "HEIGHT", -(((ResY * 74) / 100) - ResY));

iniWriter.SetIniPath("Pannels\\bandeau_listebatiment.INI");
iniWriter.WriteInteger("bandeau_listebatiment", "Y", ResY-155); // 26 aka 445
iniWriter.WriteInteger("bandeau_listebatiment", "HEIGHT", ResY);

iniWriter.SetIniPath("Pannels\\bandeau_commun.INI");
iniWriter.WriteInteger("bloc_racine", "Y", ResY -145-2); // 24 /// object or character
//iniWriter.WriteInteger("bloc_racine", "HEIGHT", 155); // 155 /// object or character

iniWriter.SetIniPath("Pannels\\Batiment_Cuisine.ini");
iniWriter.WriteInteger("BATIMENT_CUISINE", "Y", ResY-108); // 492
iniWriter.WriteInteger("BATIMENT_CUISINE_HAUT", "Y", ResY - 108-13); //479
iniWriter.WriteInteger("BATIMENT_CUISINE_ASSIETTE", "Y", ResY - 108-8); //484
iniWriter.WriteInteger("BATIMENT_CUISINE_BAS", "Y", ResY - 108-4); //488
iniWriter.WriteInteger("BATIMENT_CUISINE_PASSAGER", "Y", ResY - 108+4); //496
// iniWriter.WriteInteger("BATIMENT_CUISINE_SECTION1", "Y", ResY - 108);
//iniWriter.WriteInteger("BATIMENT_CUISINE_GAUGE1", "Y", ResY - 108);
//iniWriter.WriteInteger("BATIMENT_CUISINE_SYMBOL_1", "Y", ResY - 108);
//iniWriter.WriteInteger("BATIMENT_CUISINE_CONTEXTTEXT1", "Y", ResY - 108);

iniWriter.SetIniPath("Pannels\\Batiment_Generateur.ini");
iniWriter.WriteInteger("BATIMENT_GENERATEUR", "Y", ResY - 108); // 492
iniWriter.WriteInteger("BATIMENT_GENERATEUR_HAUT", "Y", ResY - 108 - 13); //479
iniWriter.WriteInteger("BATIMENT_GENERATEUR_BAS", "Y", ResY - 108 - 4); //488
iniWriter.WriteInteger("BATIMENT_GENERATEUR_GAUGE", "Y", ResY - 108 - 8); //483

iniWriter.SetIniPath("Pannels\\Batiment_Dynamo.ini");
iniWriter.WriteInteger("BATIMENT_DYNAMO", "Y", ResY+261); //861
iniWriter.WriteInteger("BATIMENT_DYNAMO_HAUT", "Y", ResY - 108 - 13); //479
iniWriter.WriteInteger("BATIMENT_DYNAMO_COUT", "Y", ResY - 108 - 8); //484
iniWriter.WriteInteger("BATIMENT_DYNAMO_BAS", "Y", ResY - 108 - 4); //488
iniWriter.WriteInteger("BATIMENT_DYNAMO_PASSAGER", "Y", ResY - 108 + 4); //496

iniWriter.SetIniPath("Pannels\\batiment_inventeur.ini");
iniWriter.WriteInteger("BATIMENT_INVENTEUR", "Y", ResY - 108); // 492

iniWriter.SetIniPath("Pannels\\batiment_grenier.ini");
iniWriter.WriteInteger("BATIMENT_GRENIER", "Y", ResY - 108); // 492
iniWriter.WriteInteger("BATIMENT_GRENIER_HAUT", "Y", ResY - 108 - 13); //479
iniWriter.WriteInteger("BATIMENT_GRENIER_GAUGE", "Y", ResY - 108 - 8); //483
iniWriter.WriteInteger("BATIMENT_GRENIER_BAS", "Y", ResY - 108 - 4); //488

iniWriter.SetIniPath("Pannels\\batiment_entrepot.ini");
iniWriter.WriteInteger("BATIMENT_ENTREPOT", "Y", ResY - 108); // 492
iniWriter.WriteInteger("BATIMENT_ENTREPOT_HAUT", "Y", ResY - 108 - 13); //479
iniWriter.WriteInteger("BATIMENT_ENTREPOT_GAUGE", "Y", ResY - 108 - 8); //483
iniWriter.WriteInteger("BATIMENT_ENTREPOT_BAS", "Y", ResY - 108 - 4); //488

iniWriter.SetIniPath("Pannels\\Batiment_hangar.ini");
iniWriter.WriteInteger("BATIMENT_HANGAR", "Y", ResY - 108); // 492
iniWriter.WriteInteger("HANGAR_INFO_PH", "Y", ResY - 145); // 455

iniWriter.SetIniPath("Pannels\\batiment_incubateur.ini");
iniWriter.WriteInteger("BATIMENT_INCUBATEUR", "Y", ResY - 108); // 492
iniWriter.WriteInteger("BATIMENT_INCUBATEUR_NB_OEUF_HAUT", "Y", ResY - 108 - 13); //479
iniWriter.WriteInteger("BATIMENT_INCUBATEUR_OEUFS", "Y", ResY - 108 - 8); //484
iniWriter.WriteInteger("BATIMENT_INCUBATEUR_NB_OEUF", "Y", ResY - 108 - 4); //488

iniWriter.SetIniPath("Pannels\\batiment_infirmerie.ini");
iniWriter.WriteInteger("BATIMENT_INFIRMERIE", "Y", ResY - 108); // 492
iniWriter.WriteInteger("BATIMENT_INFIRMERIE_HAUT", "Y", ResY - 108 - 13); //479
iniWriter.WriteInteger("BATIMENT_SHOWINFIRMERIE", "Y", ResY - 108 - 8); //484
iniWriter.WriteInteger("BATIMENT_INFIRMERIE_BAS", "Y", ResY - 108 - 4); //488
iniWriter.WriteInteger("BATIMENT_INFIRMERIE_PASSAGER", "Y", ResY - 108 + 4); //496

iniWriter.SetIniPath("Pannels\\batiment_manufacture.ini");
iniWriter.WriteInteger("BATIMENT_MANUFACTURE", "Y", ResY - 108); // 492

iniWriter.SetIniPath("Pannels\\Batiment_prison.ini");
iniWriter.WriteInteger("BATIMENT_PRISON", "Y", ResY - 98); // 502

iniWriter.SetIniPath("Pannels\\Batiment_Stoc.ini");
iniWriter.WriteInteger("Batiment_Stoc", "Y", ResY - 98); // 502

iniWriter.SetIniPath("Pannels\\batiment_infirmerie.ini");
iniWriter.WriteInteger("BATIMENT_INFIRMERIE", "Y", ResY - 108); // 492
iniWriter.WriteInteger("BATIMENT_INFIRMERIE_HAUT", "Y", ResY - 108 - 13); //479
iniWriter.WriteInteger("BATIMENT_SHOWINFIRMERIE", "Y", ResY - 108 - 8); //484
iniWriter.WriteInteger("BATIMENT_INFIRMERIE_BAS", "Y", ResY - 108 - 4); //488
iniWriter.WriteInteger("BATIMENT_INFIRMERIE_PASSAGER", "Y", ResY - 108 + 4); //496

iniWriter.SetIniPath("Pannels\\Batiment_Ecole.ini");
iniWriter.WriteInteger("BATIMENT_ECOLE", "Y", ResY - 108); // 492


iniWriter.SetIniPath("Pannels\\Batiment_Dortoir.ini");
iniWriter.WriteInteger("BATIMENT_DORTOIR", "Y", ResY - 108); // 492
iniWriter.WriteInteger("BATIMENT_DORTOIR_HAUT", "Y", ResY - 108 - 13); //479
iniWriter.WriteInteger("BATIMENT_SHOWDORTOIR", "Y", ResY - 108 - 8); //484
iniWriter.WriteInteger("BATIMENT_DORTOIR_BAS", "Y", ResY - 108 - 4); //488
iniWriter.WriteInteger("BATIMENT_DORTOIR_PASSAGER", "Y", ResY - 108 + 4); //496

iniWriter.SetIniPath("Pannels\\actionPH.ini");
iniWriter.WriteInteger("PARENT_ACTION", "Y", ResY - 108); // 300

iniWriter.SetIniPath("Pannels\\fiche_passager.ini");
iniWriter.WriteInteger("INFO_PASSAGER", "Y", ResY - 104); // 496

iniWriter.SetIniPath("Pannels\\chargement.ini");
iniWriter.WriteInteger("CHARGEMENT_ECRAN", "WIDTH", ResX);
iniWriter.WriteInteger("CHARGEMENT_ECRAN", "HEIGHT", ResY);

iniWriter.SetIniPath("Pannels\\fiche_perso.ini");
iniWriter.WriteInteger("ACTION_PERSO", "Y", ResY - 112); // 488
iniWriter.WriteInteger("FOND_FICHE_PERSO", "Y", ResY + 372); // 942
iniWriter.WriteInteger("CACHE_BOUTON", "Y", ResY - 103); // 497
iniWriter.WriteInteger("FICHE_PERSO_HAUT", "Y", ResY - 121); // 479
iniWriter.WriteInteger("FICHE_PERSO", "Y", ResY - 112); // 488

iniWriter.SetIniPath("Pannels\\fiche_vehicule.ini");
iniWriter.WriteInteger("VEHICULE_PERSO", "Y", ResY - 112); // 488
iniWriter.WriteInteger("VEHICULES_FOND_JAUGES1", "Y", ResY - 117); // 483
iniWriter.WriteInteger("VEHICULES_FOND_JAUGES2", "Y", ResY - 117); // 483
iniWriter.WriteInteger("VEHICULES_CACHE_BOUTON", "Y", ResY - 78); // 522
iniWriter.WriteInteger("VEHICULE_BLOCNOIR", "Y", ResY - 108); // 492
iniWriter.WriteInteger("FICHE_VEHICULE_HAUT", "Y", ResY - 121); // 479
iniWriter.WriteInteger("FICHE_VEHICULE", "Y", ResY - 112); // 488
iniWriter.WriteInteger("VEHICULE_ACTION", "Y", ResY - 112); // 488

iniWriter.SetIniPath("Pannels\\bandeau_batiment.ini");
//iniWriter.WriteInteger("bandeau_batiment", "X", ResX - 685); // 492
iniWriter.WriteInteger("bandeau_batiment", "Y", ResY - 155); //445


iniWriter.SetIniPath("Pannels\\tips.ini");
iniWriter.WriteInteger("MENU_INFO", "X", ResX/4);
iniWriter.WriteInteger("MENU_INFO", "Y", ResY/4); 
iniWriter.WriteInteger("MENU_INFO_TIPS", "WIDTH", ResX);
iniWriter.WriteInteger("MENU_INFO_TIPS", "HEIGHT", ResY); 
iniWriter.WriteInteger("MENUINGAME_BLACK_TIPS", "WIDTH", ResX);
iniWriter.WriteInteger("MENUINGAME_BLACK_TIPS", "HEIGHT", ResY); 

iniWriter.SetIniPath("Pannels\\bandeau_blurk_bas.ini"); // talking head when mission selected
iniWriter.WriteInteger("MENU_BLURK_BAS", "X", ResX/4);
iniWriter.WriteInteger("MENU_BLURK_BAS", "Y", ResY/4); 
//iniWriter.WriteInteger("MENU_BLURK_VALIDER_", "X", ResX-55); //745
//iniWriter.WriteInteger("MENU_BLURK_VALIDER_", "Y", ResY-370); //230
//iniWriter.WriteInteger("LIST_TEXTMISSION_BLURKBAS_T", "X", ResX-530); //270
//iniWriter.WriteInteger("LIST_TEXTMISSION_BLURKBAS_T", "Y", ResY-526); //75
iniWriter.WriteInteger("MENU_BLURK_BLACK", "WIDTH", ResX);
iniWriter.WriteInteger("MENU_BLURK_BLACK", "HEIGHT", ResY);

iniWriter.SetIniPath("Pannels\\bandeau_blurk_haut.ini");
iniWriter.WriteInteger("MENU_BLURK_HAUT", "WIDTH", ResX);
iniWriter.WriteInteger("MENU_BLURK_HAUT", "HEIGHT", ResY); 

iniWriter.SetIniPath("Pannels\\menumissionbrief.ini"); // after talking head when mission selected
iniWriter.WriteInteger("MENUMISSIONBRIEF", "X", ResX / 4);
iniWriter.WriteInteger("MENUMISSIONBRIEF", "Y", ResY / 4);
iniWriter.WriteInteger("MENUJOUEURS", "WIDTH", ResX);
iniWriter.WriteInteger("MENUJOUEURS", "HEIGHT", ResY);

iniWriter.SetIniPath("Pannels\\menuload.ini");
//iniWriter.WriteInteger("MENUMISSIONBRIEF", "X", ResX / 4);
//iniWriter.WriteInteger("MENUMISSIONBRIEF", "Y", ResY / 4);
iniWriter.WriteInteger("MENUCHARGER", "WIDTH", ResX);
iniWriter.WriteInteger("MENUCHARGER", "HEIGHT", ResY);

iniWriter.SetIniPath("Pannels\\menuplayers.ini");
iniWriter.WriteInteger("MENUJOUEURS", "WIDTH", ResX);
iniWriter.WriteInteger("MENUJOUEURS", "HEIGHT", ResY);

iniWriter.SetIniPath("Pannels\\campagne.ini");
iniWriter.WriteInteger("ROOT_CAMPAGNE", "WIDTH", ResX);
iniWriter.WriteInteger("ROOT_CAMPAGNE", "HEIGHT", ResY);

iniWriter.SetIniPath("Pannels\\menuselectmission.ini");
iniWriter.WriteInteger("ROOT_CAMPAGNE", "X", ResX);
iniWriter.WriteInteger("ROOT_CAMPAGNE", "Y", ResY);

iniWriter.SetIniPath("Pannels\\info_bloc.ini");
iniWriter.WriteInteger("info_bloc", "Y", ResY-98);

iniWriter.SetIniPath("Pannels\\SelectedGroupePH.ini");
iniWriter.WriteInteger("SELECTEDGROUPE_PHROOT", "Y", ResY-110); //490
iniWriter.WriteInteger("GROUPEACTION_PERSO", "Y", ResY-112); //488

iniWriter.SetIniPath("Pannels\\construction.ini");
iniWriter.WriteInteger("CONSTRUIRE_FOND", "Y", ResY-108); //492
iniWriter.WriteInteger("CONSTRUCTION_ROOT", "Y", ResY-87); //513
iniWriter.WriteInteger("CONSTRUIRE_CACHE_BOUTON", "Y", ResY-78); //522
iniWriter.WriteInteger("CONSTRUIRE_CANCEL", "Y", ResY-124); //476
iniWriter.WriteInteger("CONSTRUIRE_INFORMATION_HAUT", "Y", ResY-125); //475

iniWriter.SetIniPath("Pannels\\construction_.ini");
iniWriter.WriteInteger("CONSTRUIRE_FOND", "Y", ResY-108); //492
iniWriter.WriteInteger("CONSTRUCTION_ROOT", "Y", ResY-87); //513
iniWriter.WriteInteger("CONSTRUIRE_CACHE_BOUTON", "Y", ResY-78); //522
iniWriter.WriteInteger("CONSTRUIRE_CANCEL", "Y", ResY-124); //476
iniWriter.WriteInteger("CONSTRUIRE_INFORMATION_HAUT", "Y", ResY-125); //475
iniWriter.WriteInteger("CONSTRUIRE_INFO", "Y", ResY-34); //566

iniWriter.SetIniPath("Pannels\\inventairePH.ini");
iniWriter.WriteInteger("inventaire_root", "Y", ResY-110); //496
iniWriter.WriteInteger("INVENTAIREPH_pages", "Y", ResY-120); //480

iniWriter.SetIniPath("Pannels\\inventaireVehicule.ini");
iniWriter.WriteInteger("inventairevehicule_root", "Y", ResY-104); //496
iniWriter.WriteInteger("inventairevehicule_pages", "Y", ResY-115); //485

iniWriter.SetIniPath("Pannels\\GroupeBatiment.ini");
iniWriter.WriteInteger("GROUPE_BATIMENTROOT", "Y", ResY-104); //490
iniWriter.WriteInteger("GROUPE_BATIMENT_PAGES", "Y", ResY-120); //480

iniWriter.SetIniPath("Pannels\\GroupePH.ini"); // group image
iniWriter.WriteInteger("GROUPE_PHROOT", "Y", ResY - 110); //490
iniWriter.WriteInteger("GROUPE_PH_pages", "Y", ResY - 120); //480

iniWriter.SetIniPath("Pannels\\info_ph.ini");
iniWriter.WriteInteger("INFO_PH1", "Y", ResY - 145); //455
iniWriter.WriteInteger("INFO_PH2", "Y", ResY - 98); //502

iniWriter.SetIniPath("Pannels\\rollover.ini");
iniWriter.WriteInteger("ROOT_ROLLOVER", "Y", ResY - 400); //200

iniWriter.SetIniPath("Pannels\\popup_inventairePH.ini");
iniWriter.WriteInteger("popupinventaire_root", "Y", ResY - 400); //200

iniWriter.SetIniPath("Pannels\\liste_fanions.ini");
iniWriter.WriteInteger("LISTEFANIONS_ROOT", "Y", ResY - 102); //498

iniWriter.SetIniPath("Pannels\\FANIONS.ini");
iniWriter.WriteInteger("fanion_bloc_bandeau", "Y", ResY - 102); //498

iniWriter.SetIniPath("Pannels\\couvaison.ini");
iniWriter.WriteInteger("fanion_bloc_bandeau", "Y", ResY - 98); //502

iniWriter.SetIniPath("Pannels\\menuloadsaved.ini");
iniWriter.WriteInteger("MENUCHARGER", "WIDTH", ResX);
iniWriter.WriteInteger("MENUCHARGER", "HEIGHT", ResY);

iniWriter.SetIniPath("Pannels\\cartemission_tutorial.ini");
iniWriter.WriteInteger("CARTE_MISSION_TUTORIAL", "WIDTH", ResX);
iniWriter.WriteInteger("CARTE_MISSION_TUTORIAL", "HEIGHT", ResY);

iniWriter.SetIniPath("Pannels\\victory.ini");
iniWriter.WriteInteger("VICTORY", "WIDTH", ResX);
iniWriter.WriteInteger("VICTORY", "HEIGHT", ResY);
//iniWriter.WriteInteger("CARTE_MISSION_TUTORIAL4", "WIDTH", ResX); // bigger img
//iniWriter.WriteInteger("CARTE_MISSION_TUTORIAL4", "HEIGHT", ResY);



if (true){
    //injector::WriteMemory<DWORD>(0x416921, (DWORD)((char*)"Auto"), true); // options to show screen res text
    injector::WriteMemory<unsigned char>(0x40cc55, 0x90, true); // No CD
    injector::WriteMemory<unsigned char>(0x40cc56, 0x90, true); // No CD

    // Game Render ! Reversed in IDA. Nice over here ;)
    injector::WriteMemory(0x411A7D, ResX, true); // 800 0x320 2003
    injector::WriteMemory(0x411A78, ResY, true); // 600 0x258 5802

// Mouse cursor render
    injector::WriteMemory(0x60962F, ResX, true); // 800
    injector::WriteMemory(0x609637, ResY, true); // 600

// [UNKNOWN] Button acceptance region (test)
    injector::WriteMemory(0x401444, ResX, true); // 800
    injector::WriteMemory(0x401455, ResY, true); // 600

// [UNKNOWN] 
    injector::WriteMemory(0x401478, ResX, true); // 800
    injector::WriteMemory(0x40148b, ResY, true); // 600

    injector::WriteMemory(0x40dc26, ResX, true); // 800 window pos

    injector::WriteMemory(0x48fd4e, ResX, true); // 800 0x320 2003

    injector::WriteMemory(0x48fdb9, ResX, true); // 800 0x320 2003
    injector::WriteMemory(0x678d64, ResX, true); // 800 0x320 2003

    // [UNKNOWN] 12c (300) 320 (800)
    injector::WriteMemory(0x40dd49, ResX, true); // 800
    injector::WriteMemory(0x40dd44, ResY, true); // 600

 // [UNKNOWN] Button acceptance region (test 2)
    injector::WriteMemory(0x40dc6f, ResX, true); // 800
    injector::WriteMemory(0x40dc74, ResY, true); // 600

// [UNKNOWN] Button acceptance region (test 3)
    injector::WriteMemory(0x40dcfa, ResX, true); // 800
    injector::WriteMemory(0x40dcff, ResY, true); // 600

// [UNKNOWN] Button acceptance region (test 4)
    injector::WriteMemory(0x40de13, ResX, true); // 800
    injector::WriteMemory(0x40de18, ResY, true); // 600

// [UNKNOWN] Button acceptance region (test 5)
    injector::WriteMemory(0x40de58, ResX, true); // 800
    injector::WriteMemory(0x40de5d, ResY, true); // 600

// [UNKNOWN] Button acceptance region (test 6) ! reversed in IDA. Nice over here ;)
    injector::WriteMemory(0x412b80, ResX, true); // 800
    injector::WriteMemory(0x412b7b, ResY, true); // 600

// [UNKNOWN] Button acceptance region (test 7)
    injector::WriteMemory(0x412cfd, ResX, true); // 800
    injector::WriteMemory(0x412d02, ResY, true); // 600

/* after a long break... NEW CODE!*/
    injector::WriteMemory(0x4014cd, ResY, true); // 600
    injector::WriteMemory(0x40dcbb, ResY, true); // 600
    injector::WriteMemory(0x40dd8e, ResY, true); // 600
    injector::WriteMemory(0x40ddd4, ResY, true); // 600
    injector::WriteMemory(0x4f4df4, ResY, true); // 600
    injector::WriteMemory(0x64B848, ResY, true); // 600
    injector::WriteMemory(0x64BB22, ResY, true); // 600

}

}


BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        Init();
    }
    return TRUE;
}