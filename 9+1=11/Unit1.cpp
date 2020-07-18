//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <iostream.h>
#include <fstream.h>
#include <math.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tmain_form *main_form;
//---------------------------------------------------------------------------

//===========================================================================
int minutes = 0, seconds = 0;

const int max_len = 36;

struct coord
{ int x; int y; int pos; bool turn; };
coord main_path[max_len];

coord player_stat[4];
struct PImage
{   TImage *image; };
PImage player_img_list[4];

int dice = 0, dice_bk;
int active_player = 0, move_count = 30;

//---------------------------------------------------------------------------
void indicator (TImage *img, TImageList *list, int id);

void load_file(char *filename, coord *path)
{
    ifstream f(filename); int indx;
    for (int i=0; i<max_len; i++)
    {
        f>>indx;
        f>>path[indx].x>>path[indx].y;
    }
    f.close();
}
//---------------------------------------------------------------------------
__fastcall Tmain_form::Tmain_form(TComponent* Owner)
    : TForm(Owner)
{
    DoubleBuffered = true;
    randomize();

    for (int i=0; i<max_len; i++)
    {
        main_path[i].pos = i;
    }
    for (int i=0; i<4; i++)
    {
        player_stat[i].pos = 0;
        player_stat[i].turn = true;
    }
    load_file(".\\path.txt", main_path);

    player_img_list[0].image = player1; // player images in massive
    player_img_list[1].image = player2;
    player_img_list[2].image = player3;
    player_img_list[3].image = player4;

    //active_player = 1+random(6);
    indicator(Image_indicator, indicator_list, active_player);
    indicator(Image_dice, dice_list, dice);
}
//---------------------------------------------------------------------------

//===========================================================================    advanced move alg
void move_P(TImage *img, const coord &dest, int step, bool &need_move, int id)
{
    int midX = img->Left + img->Width/2;
    int midY = img->Top + img->Height/2 + img->Height/4;
    int dx, dy; // dest.x - midX; dest.y - midY;
    //===========
    switch (id)
    {
        case 0: { dx = dest.x - midX; dy = dest.y-15 - midY; break; }
        case 1: { dx = dest.x-15 - midX; dy = dest.y+5 - midY; break; }
        case 2: { dx = dest.x - midX; dy = dest.y+15 - midY; break; }
        case 3: { dx = dest.x+15 - midX; dy = dest.y-5 - midY; break; }
    }
    //===========
    if(abs(dx)<step && abs(dy)<step)
    {
        need_move = false;

        if (player_stat[active_player].pos >= dice_bk)
        {
            if (dice_bk == 4) dice_bk = 8;
            if (dice_bk == 17) dice_bk = 14;
            if (dice_bk == 20) dice_bk = 27;
            if (dice_bk == 23) player_stat[active_player].turn = false;

            player_stat[active_player].pos = dice_bk;
            main_form->Timer_Pmove->Enabled = false; return;
        }
        player_stat[active_player].pos+=1;
        return;
    }
    //===========
    need_move = true;
    double angle = atan2(dy, dx);
    img->Left += step*cos(angle);
    img->Top += step*sin(angle);
}
//---------------------------------------------------------------------------
void __fastcall Tmain_form::Timer_timeTimer(TObject *Sender)
{
    Time->Caption = "Time "+IntToStr(minutes)+":"+IntToStr(seconds);
    if (seconds+1 < 60) seconds+=1;
    else { minutes+=1; seconds = 0; }
}
//---------------------------------------------------------------------------
void indicator(TImage *img, TImageList *list, int id)
{
    TRect rect(0,0,img->Width, img->Height);
    img->Canvas->Brush->Color = clLime;
    img->Canvas->FillRect(rect);
    list->Draw(img->Canvas, 0, 0, id);
}
//---------------------------------------------------------------------------
void __fastcall Tmain_form::Timer_diceTimer(TObject *Sender)
{
    //if (move_count<=20) Timer_Pmove->Enabled = true; // delay after dice
    if (move_count == 0)
    {
        Timer_Pmove->Enabled = true; // delay after dice
        move_count = 30;

        active_player = (active_player+1)%4;

        if (player_stat[active_player].turn == false)
        { player_stat[active_player].turn = true; active_player = (active_player+1)%4; };

        dice_bk = player_stat[active_player].pos+dice;
        //////if (dice_bk > 35) dice_bk = 36;
        indicator(Image_indicator, indicator_list, active_player);

        dice_val->Caption = "None";
        indicator(Image_dice, dice_list, 0);
        Timer_dice->Enabled = false;
    }
    else
    {
        move_count--;
    }
    //active_player;
}
//---------------------------------------------------------------------------

//========================================= Button out of Label =============    Dice
void __fastcall Tmain_form::Label_dice_buttonMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Label_dice_button->Top+=3;
    Label_dice_button->Font->Color = clGreen;

    if (move_count == 30)
    {
        srand(time(NULL));
        dice = 1+random(6);
        dice_val->Caption = IntToStr(dice);
        indicator(Image_dice, dice_list, dice);

        Timer_dice->Enabled = true;
    }
}
//-----
void __fastcall Tmain_form::Label_dice_buttonMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Label_dice_button->Top-=3;
    Label_dice_button->Font->Color = clWindowText;
}
//===========================================================================

//---------------------------------------------------------------------------

void __fastcall Tmain_form::Timer_PmoveTimer(TObject *Sender)
{
//
    bool need_move;
    if (!(((player_stat[active_player].pos==4) && (dice_bk==4))||((player_stat[active_player].pos==17) && (dice_bk==17))||((player_stat[active_player].pos==20) && (dice_bk==20)))) move_P(player_img_list[active_player].image, main_path[player_stat[active_player].pos], 10, need_move, active_player);
    else if ((player_stat[active_player].pos==4) && (dice_bk==4)) move_P(player_img_list[active_player].image, main_path[8], 10, need_move, active_player);
    else if ((player_stat[active_player].pos==20) && (dice_bk==20)) move_P(player_img_list[active_player].image, main_path[27], 10, need_move, active_player);
    else if ((player_stat[active_player].pos==17) && (dice_bk==17)) move_P(player_img_list[active_player].image, main_path[14], 10, need_move, active_player);
    if (player_stat[active_player].pos>35)
    {
        switch (active_player)
        {
            case 0:
            {
                Timer_Pmove->Enabled = false; Timer_time->Enabled = false;
                ShowMessage("WIN RED");
                break; // If RED wins
            }
            case 1:
            {
                Timer_Pmove->Enabled = false; Timer_time->Enabled = false;
                ShowMessage("WIN YELLOW");
                break; // If YELLOW wins
            }
            case 2:
            {
                Timer_Pmove->Enabled = false; Timer_time->Enabled = false;
                ShowMessage("WIN  - GREEN");
                break; // If GREEN wins
            }
            case 3:
            {
                Timer_Pmove->Enabled = false; Timer_time->Enabled = false;
                ShowMessage("WIN BLUE");
                break; // If BLUE wins
            }
        }
    }
}
//---------------------------------------------------------------------------


