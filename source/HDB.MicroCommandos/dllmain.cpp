#include "stdafx.h"
#include <fstream>

bool bHACKMEfixes;

// Simple Release-safe file log (the mod's spdlog is _DEBUG-only). Writes
// <gamedir>\HDB.MicroCommandos.log so cheats/diagnostics are checkable without a debugger.
static void LogLine(const std::string& s)
{
    static const std::string path = GetExeModulePath<std::string>() + "HDB.MicroCommandos.log";
    std::ofstream f(path, std::ios::app);
    if (f) f << s << std::endl;
}

/* ---- Middle-mouse camera rotation -----------------------------------------------
   Hooks the per-frame input handler FUN_004135a0 (single call site 0x00414771).
   When Ctrl is held that handler ROTATES the camera (vtable +0x20 yaw-, +0x24 yaw+)
   reading the DirectInput keyboard buffer (ptr @0x00703798); otherwise it pans.
   While MMB is held we:
     (1) lock the cursor to its press point (recenter each frame) so it never hits
         the screen edge -- this also keeps the game's edge-scroll quiet; and
     (2) forge Ctrl+Left/Right into the live key buffer, scaling the per-frame
         rotate step (float @0x006d88e8, default 0.02 rad) by mouse speed so fast
         drags rotate faster (proportional feel).
   mchd.ini [MAIN]: MmbRotate (1), MmbRotateDeadzone (2),
   MmbRotateSensitivity (0.15), MmbRotateMaxFactor (8.0). */
typedef void (__cdecl* tCamInput)();
static tCamInput oCamInput      = nullptr;
static bool      bMmbRotate      = true;
static bool      bCheatVictory   = false;   // F10 = show victory screen (testing)
// cInterface_VictoryEnd::SetVisibleAndValid(bool) @0x612f00 (__thiscall); the global
// instance pointer is at 0x00704cc8. Calling with true shows the victory UI.
typedef void (__thiscall* tVictoryShow)(void* self, bool show);
static int       iMmbDeadzone    = 2;
static float     fMmbSens        = 0.15f;
static float     fMmbMaxFactor   = 8.0f;

static float* const pCamStep = (float*)0x006d88e8;  // per-frame camera step passed to rotate/pan

static void __cdecl CamInput_Hook()
{
    static bool  haveStep = false;
    static float origStep = 0.02f;
    static POINT anchor   = { 0, 0 };
    static bool  dragging = false;
    if (!haveStep) { origStep = *pCamStep; haveStep = true; }

    bool scaled = false;
    if (bMmbRotate && (GetAsyncKeyState(VK_MBUTTON) & 0x8000))
    {
        POINT p; GetCursorPos(&p);
        if (!dragging) { anchor = p; dragging = true; }
        LONG dx = p.x - anchor.x;
        SetCursorPos(anchor.x, anchor.y);            // lock cursor to the press point

        BYTE* kb = *(BYTE**)0x00703798;              // live DI keyboard state
        if (kb && (dx < -iMmbDeadzone || dx > iMmbDeadzone))
        {
            float factor = (dx < 0 ? -dx : dx) * fMmbSens;
            if (factor > fMmbMaxFactor) factor = fMmbMaxFactor;
            *pCamStep = origStep * factor;           // speed-proportional rotation
            scaled = true;
            kb[0x1d] = 0x80;                          // DIK_LCONTROL -> rotate branch
            kb[dx < 0 ? 0xcb : 0xcd] = 0x80;         // DIK_LEFT / DIK_RIGHT
        }
    }
    else dragging = false;

    if (bCheatVictory) {                             // F9 -> show victory screen
        // The game's DI keyboard buffer is filtered to keys it uses (F9 never appears),
        // so try GetAsyncKeyState too (works for MMB, likely works for F9). Trigger on
        // either; log which fired so we know the right input source going forward.
        static bool prev = false;
        bool gaks = (GetAsyncKeyState(VK_F9) & 0x8000) != 0;
        BYTE* kb = *(BYTE**)0x00703798;
        bool dik = (kb != nullptr) && (kb[0x43] & 0x80) != 0;
        bool k = gaks || dik;
        if (k && !prev) {
            void* vic = *(void**)0x00704cc8;
            LogLine(format("[HDB] F9 victory (gaks=%d dik=%d): vic=%p", (int)gaks, (int)dik, vic));
            if (vic) ((tVictoryShow)0x00612f00)(vic, true);
        }
        prev = k;
    }

    if (oCamInput) oCamInput();
    if (scaled) *pCamStep = origStep;                // restore (keep keyboard rotate normal)
}

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
    bMmbRotate    = iniReader.ReadInteger("MAIN", "MmbRotate", 1) != 0;
    iMmbDeadzone  = iniReader.ReadInteger("MAIN", "MmbRotateDeadzone", 2);
    fMmbSens      = iniReader.ReadFloat("MAIN", "MmbRotateSensitivity", 0.15f);
    fMmbMaxFactor = iniReader.ReadFloat("MAIN", "MmbRotateMaxFactor", 8.0f);
    bCheatVictory = iniReader.ReadInteger("Patchworks", "CheatVictory", 0) != 0; // F9 -> victory screen

    if (!ResX || !ResY)
        std::tie(ResX, ResY) = GetDesktopRes();

    // Offset to center an 800x600-designed panel/root on the current resolution.
    // (The old `X = ResX/4` only centers at 1600 wide; this is correct at any res.)
    int centerX = (ResX - 800) / 2;
    int centerY = (ResY - 600) / 2;
    LogLine(format("[HDB] Init %dx%d center(%d,%d) Mmb=%d CheatVictory=%d HACKME=%d",
                   ResX, ResY, centerX, centerY,
                   (int)bMmbRotate, (int)bCheatVictory, (int)bHACKMEfixes));

/* GAME EXE SCREEN RES */

/* GUI FILES BEEFUP */
CIniReader iniWriter("Pannels\\menuprincipal.ini"); // FIXED: was "..\\Pannels\\..." -> wrong dir, so the main menu was never actually updated
// Center the whole 800x600 main-menu composition. Every menu element hangs off one
// of three roots (logo / interface-art band / buttons+text), so offsetting just the
// roots by (centerX, centerY) moves everything together and keeps the art aligned.
// (Replaces the old per-element percentage "HACK", which distorted the cascade, and
// the right-edge logo anchoring that never ran due to the path bug.)
iniWriter.WriteInteger("MENUPRINCIPAL", "WIDTH", 800);
iniWriter.WriteInteger("MENUPRINCIPAL", "HEIGHT", 600);
iniWriter.WriteInteger("MENUPRINCIPAL", "X", centerX);              // buttons + text labels
iniWriter.WriteInteger("MENUPRINCIPAL", "Y", centerY);
iniWriter.WriteInteger("MAINMENU_PRINCIPALEROOT", "X", centerX);    // interface-art band (orig 0,0)
iniWriter.WriteInteger("MAINMENU_PRINCIPALEROOT", "Y", centerY);
iniWriter.WriteInteger("MAINMENU_LOGOMICROROOT", "X", 190 + centerX); // logo (orig X=190,Y=0)
iniWriter.WriteInteger("MAINMENU_LOGOMICROROOT", "Y", centerY);
// Keep "Current Player" label + value pinned to the screen top-left: cancel out the
// centered MENUPRINCIPAL parent so they sit at absolute (10,33) / (150,33).
iniWriter.WriteInteger("MAINMENU_PLAYERNAME_T", "X", 10 - centerX);   // orig 10,33
iniWriter.WriteInteger("MAINMENU_PLAYERNAME_T", "Y", 33 - centerY);
iniWriter.WriteInteger("MAINMENU_PLAYERNAME", "X", 150 - centerX);    // orig 150,33
iniWriter.WriteInteger("MAINMENU_PLAYERNAME", "Y", 33 - centerY);
// menu_interfaceprincipale01 is the decorative bar for the player header (not part
// of the button arc, which is pieces 02/03). Pin it top-left with the player name.
iniWriter.WriteInteger("MAINMENU_PRINCIPALE01", "X", 0 - centerX);    // -> absolute (0,3)
iniWriter.WriteInteger("MAINMENU_PRINCIPALE01", "Y", 3 - centerY);

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

// MENU_OUI_NON is the yes/no modal container; its children are positioned relative to
// it. Center the dialog by offsetting each child from its ORIGINAL 800x600 position by
// (centerX, centerY). Replaces the old right-edge anchoring + resolution-specific HACK.
//
// DO NOT RESIZE THIS CONTAINER. It used to be written to ResX/ResY, and that single
// pair of lines was what pushed the whole dialog off screen.
//
// The game centres a panel by converting (800 - ResX)/2 to float, and it does the
// conversion UNSIGNED: `fild qword` over a 32-bit value whose high dword was zeroed
// (cInterfaceYesNoBox::SetVisibleAndValid, 00614c80). While a panel fits inside 800x600
// the offset is >= 0 and nothing shows. Resize the panel to the screen and the offset
// goes negative -- at 1920 wide, -560 -- which read as unsigned is 4294966784. Every
// child inherits the container's position, so the dialog lands ~4 billion pixels away.
//
// MEASURED in the recompilation (microcommandos-decomp/recomp, scratchpad/click.py
// reports the quit widget tree). With WIDTH/HEIGHT written:
//     ancestor x=4294966784.00  raw 4f7ffffe      LIST x=4294967808.00
// With them left alone:
//     ancestor x=0.00           raw 00000000      LIST x=875.00 y=510.00
// and 875 = 315 + centerX(560), 510 = 270 + centerY(240) -- exactly what the child
// offsets below already intend. The children were always right; the resize was the bug.
//
// The full-screen dimming behind the dialog is MENUINGAME_BLACK_YESNO, written above, so
// nothing here needs the container to cover the screen.

iniWriter.WriteInteger("LIST_MENU_OUI_NON_TEXT", "X", 315 + centerX);
iniWriter.WriteInteger("LIST_MENU_OUI_NON_TEXT", "Y", 270 + centerY);
iniWriter.WriteInteger("MENU_OUI_NON_GAUCHE", "X", 230 + centerX); // YES/NO bg left
iniWriter.WriteInteger("MENU_OUI_NON_GAUCHE", "Y", 240 + centerY);
iniWriter.WriteInteger("MENU_OUI_NON_DROIT", "X", 486 + centerX);  // YES/NO bg right
iniWriter.WriteInteger("MENU_OUI_NON_DROIT", "Y", 240 + centerY);
iniWriter.WriteInteger("MENU_NON", "X", 254 + centerX);
iniWriter.WriteInteger("MENU_NON", "Y", 405 + centerY);
iniWriter.WriteInteger("MENU_OUI", "X", 458 + centerX);
iniWriter.WriteInteger("MENU_OUI", "Y", 396 + centerY);

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
iniWriter.WriteInteger("MENU_INFO", "X", centerX); // mission objectives panel - center the 800x600 root
iniWriter.WriteInteger("MENU_INFO", "Y", centerY);
iniWriter.WriteInteger("MENU_INFO_TIPS", "WIDTH", ResX);
iniWriter.WriteInteger("MENU_INFO_TIPS", "HEIGHT", ResY); 
iniWriter.WriteInteger("MENUINGAME_BLACK_TIPS", "WIDTH", ResX);
iniWriter.WriteInteger("MENUINGAME_BLACK_TIPS", "HEIGHT", ResY); 

iniWriter.SetIniPath("Pannels\\bandeau_blurk_bas.ini"); // talking head when mission selected
iniWriter.WriteInteger("MENU_BLURK_BAS", "X", centerX); // talking-head panel - center 800x600 root
iniWriter.WriteInteger("MENU_BLURK_BAS", "Y", centerY);
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
iniWriter.WriteInteger("MENUMISSIONBRIEF", "X", centerX); // mission brief - center 800x600 root
iniWriter.WriteInteger("MENUMISSIONBRIEF", "Y", centerY);
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

// VRAM-tier resolution cap (FUN_004013d0): clamps requested res to 800x600 on
// low-VRAM tiers (`if (800 < width){ width=800; height=600; }`). Raising the cap
// to ResX/ResY. NOTE: the top VRAM tier (>=56MB) has no clamp, so modern GPUs
// bypass this anyway -- harmless + correct.
    injector::WriteMemory(0x401444, ResX, true); // 800
    injector::WriteMemory(0x401455, ResY, true); // 600
    injector::WriteMemory(0x401478, ResX, true); // 800
    injector::WriteMemory(0x40148b, ResY, true); // 600

// Editor/debug child-window layout (FUN_0040dbe0, gated by editor flag
// [DAT_00703378+0x2a420]==1): SetWindowPos coords for tool windows 0x84..0x90.
// INERT during normal fullscreen gameplay; only affects the in-engine editor.
    injector::WriteMemory(0x40dc26, ResX, true); // 800 window pos

    // REMOVED three coincidental 0x320 (=800) sites that are NOT resolution values:
    //   0x48fd4e / 0x48fdb9 : FUN_0048fd40 allocates a fixed 800-byte struct
    //       (0x20 header + 0xc0 dwords); 0x320 is sizeof(struct). The 0xc0 copy
    //       count is hardcoded, so it can't scale. ResX<800 -> heap overflow.
    //   0x678d64 : `SUB ESP,0x320` in the MSVC CRT error handler (FUN_00678d5f,
    //       "Microsoft Visual C++ Runtime Library"); 0x320 is its stack frame
    //       (local buffers 260+540=800). ResX<800 -> stack corruption.
    // All were harmless only because widescreen ResX is normally >=800.

    // (continued) editor/debug child-window layout (FUN_0040dbe0) -- inert in game
    injector::WriteMemory(0x40dd49, ResX, true); // 800
    injector::WriteMemory(0x40dd44, ResY, true); // 600
    injector::WriteMemory(0x40dc6f, ResX, true); // 800
    injector::WriteMemory(0x40dc74, ResY, true); // 600
    injector::WriteMemory(0x40dcfa, ResX, true); // 800
    injector::WriteMemory(0x40dcff, ResY, true); // 600
    injector::WriteMemory(0x40de13, ResX, true); // 800
    injector::WriteMemory(0x40de18, ResY, true); // 600
    injector::WriteMemory(0x40de58, ResX, true); // 800
    injector::WriteMemory(0x40de5d, ResY, true); // 600

// Main game window size (FUN_00412b50): default SetWindowPos extent + the
// DAT_00704b5c/DAT_00704b60 backbuffer dims (800x600 -> ResX/ResY). GENUINE.
    injector::WriteMemory(0x412b80, ResX, true); // 800
    injector::WriteMemory(0x412b7b, ResY, true); // 600
    injector::WriteMemory(0x412cfd, ResX, true); // 800
    injector::WriteMemory(0x412d02, ResY, true); // 600

/* after a long break... NEW CODE!*/
    injector::WriteMemory(0x4014cd, ResY, true); // 600 -> VRAM-tier res cap (FUN_004013d0)
    injector::WriteMemory(0x40dcbb, ResY, true); // 600 -> editor child-window layout (FUN_0040dbe0)
    injector::WriteMemory(0x40dd8e, ResY, true); // 600 -> editor child-window layout
    injector::WriteMemory(0x40ddd4, ResY, true); // 600 -> editor child-window layout
    // REMOVED 0x4f4df4 / 0x64B848 / 0x64BB22: these "600"s are SOURCE LINE NUMBERS
    // passed to the debug allocator FUN_00420350(size,...,file,line) -- e.g.
    // FUN_00420350(0x88,1,0x1b,1,"...mcLib\\src\\overlay...",600). Not resolution;
    // patching them only corrupted leak-tracking metadata. (See audit notes.)

/* Tooltip / rollover box screen-bounds clamp (FLOAT based -> not caught by the
   integer res patches above; this is why tooltips stuck at the old 800x600 edge).
   Reversed @ 0x0064cf** : the engine positions a text box at the mouse, then
   clamps it on screen:
       if (X + w > 800.0f @0x6bedf8) X = 800.0f - w;   // right edge
       if (Y + h > 600.0f @0x6be470) Y = 600.0f - h;   // bottom edge
   - 800.0f @0x6bedf8 is used ONLY by this clamp -> overwrite the const directly.
   - 600.0f @0x6be470 is SHARED with ~60 unrelated FSUB math ops -> do NOT edit
     it globally; instead repoint just this clamp's two operands (FCOMP @0x64cfc4
     and FLD @0x64cfd1, disp32 at +2) to a DLL-resident ResY float.            */
    injector::WriteMemory<float>(0x6bedf8, (float)ResX, true); // X clamp: 800.0 -> ResX

    static float fTooltipResY;
    fTooltipResY = (float)ResY;                                 // Y clamp height, ResY
    injector::WriteMemory<DWORD>(0x64cfc6, (DWORD)(uintptr_t)&fTooltipResY, true); // FCOMP operand
    injector::WriteMemory<DWORD>(0x64cfd3, (DWORD)(uintptr_t)&fTooltipResY, true); // FLD operand

    // Middle-mouse camera rotation: redirect the input handler's single call site
    // (CALL @0x00414771 -> FUN_004135a0). MakeCALL returns the original target.
    if (bMmbRotate || bCheatVictory)
        oCamInput = (tCamInput)injector::MakeCALL(0x00414771, CamInput_Hook, true).get();

    /* Victory talking-head 3D viewport (cInterface_VictoryEnd::CreateViewports @0x614860).
       Four hardcoded 800x600-relative spots for the character render -> they stay top-left
       while the victory device is centered. Shift each by (centerX, centerY) so the head
       tracks the centered device. Coords are PUSH-imm32 floats into FUN_00403940(x,y,w,h);
       operand = instruction+1. If alignment is off, the device uses a different offset --
       tune these 8 addresses (or set [Patchworks] HACKMEfixes=0 to disable). */
    if (bHACKMEfixes) {
        injector::WriteMemory<float>(0x6148e9, 122.0f + centerX, true); // A x
        injector::WriteMemory<float>(0x6148e4,  85.0f + centerY, true); // A y
        injector::WriteMemory<float>(0x61498a,  30.0f + centerX, true); // B x
        injector::WriteMemory<float>(0x614985,  30.0f + centerY, true); // B y
        injector::WriteMemory<float>(0x614a1c, 575.0f + centerX, true); // C x
        injector::WriteMemory<float>(0x614a17, 490.0f + centerY, true); // C y
        injector::WriteMemory<float>(0x614b9a, 122.0f + centerX, true); // D x
        injector::WriteMemory<float>(0x614b95,  85.0f + centerY, true); // D y
    }

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