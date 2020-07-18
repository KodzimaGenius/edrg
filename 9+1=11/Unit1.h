//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class Tmain_form : public TForm
{
__published:	// IDE-managed Components
    TImage *background;
    TImage *player1;
    TImage *player2;
    TImage *player3;
    TImage *player4;
    TTimer *Timer_dice;
    TTimer *Timer_time;
    TLabel *Time;
    TLabel *Label_dice_button;
    TLabel *dice_val;
    TImageList *indicator_list;
    TImage *Image_indicator;
    TTimer *Timer_Pmove;
    TImage *Image_dice;
    TImageList *dice_list;
    TLabel *Label1;
    void __fastcall Timer_timeTimer(TObject *Sender);
    void __fastcall Timer_diceTimer(TObject *Sender);
    void __fastcall Label_dice_buttonMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Label_dice_buttonMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Timer_PmoveTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall Tmain_form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tmain_form *main_form;
//---------------------------------------------------------------------------
#endif
