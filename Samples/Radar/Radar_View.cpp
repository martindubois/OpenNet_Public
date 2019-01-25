
// Author   KMS - Martin Dubois, ing.
// Product  OpenNet
// File     Samples/Radar/Radar_View.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <assert.h>
#include <stdint.h>

// ===== Samples/Radar ======================================================
#include "Radar_View.h"

// Static function declaration
/////////////////////////////////////////////////////////////////////////////

static void Picture_Update(System::Windows::Forms::PictureBox ^ aPicture, const unsigned char * aData);

namespace Radar
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    Radar_View::Radar_View(void) : mSource(false)
    {
        InitializeComponent();

        mC = new Radar_Controller();
    }

    // Protected
    /////////////////////////////////////////////////////////////////////////

    Radar_View::~Radar_View()
    {
        assert(NULL != mC);

        if (components)
        {
            delete components;
        }

        delete mC;
    }

    // Private
    /////////////////////////////////////////////////////////////////////////

    void Radar_View::Pictures_Update()
    {
        assert(NULL != mC);

        if (mSource)
        {
            Picture_Update(mPict_AB, mC->GetSrcAB());
            Picture_Update(mPict_CD, mC->GetSrcCD());
        }
        else
        {
            Picture_Update(mPict_AB, mC->GetDstAB());
            Picture_Update(mPict_CD, mC->GetDstCD());
        }
    }

    // ===== Form ===========================================================

    System::Void Radar_View::Radar_View_Load(System::Object ^ aS, System::EventArgs ^ aE)
    {
        assert(NULL != mC);

        mBtn_And->Checked = mC->GetAnd();
        mBtn_Or ->Checked = mC->GetOr ();

        mBtn_Start->Enabled = true ;
        mBtn_Stop ->Enabled = false;

        mNum_A->Value = mC->GetDstA();
        mNum_B->Value = mC->GetDstB();
        mNum_C->Value = mC->GetDstC();
        mNum_D->Value = mC->GetDstD();

        mNum_Fade->Value = mC->GetFade();
        mNum_Gain->Value = mC->GetGain();

        Pictures_Update();
    }

    // ===== Button =========================================================

    System::Void Radar_View::mBtn_Start_Click(System::Object ^ aS, System::EventArgs ^ aE)
    {
        assert(NULL != mC);

        mLabel_Status->Text = "Starting";

        mBtn_Start->Enabled = false;

        if (mC->Start())
        {
            mTimer->Start();

            mBtn_Stop->Enabled = true;

            mLabel_Status->Text = "Started";
        }
        else
        {
            System::Windows::Forms::MessageBox::Show("Two OpenNet adapter are needed");
            mBtn_Start->Enabled = true;

            mLabel_Status->Text = "Start error!";
        }
    }

    System::Void Radar_View::mBtn_Stop_Click(System::Object ^ aS, System::EventArgs ^ aE)
    {
        assert(NULL != mC);

        mLabel_Status->Text = "Stopping";

        mBtn_Stop->Enabled = false;

        mTimer->Stop();
        mC    ->Stop();

        mBtn_Start->Enabled = true;

        mLabel_Status->Text = "Stopped";
    }

    // ===== ComboBox =======================================================

    System::Void Radar_View::mCombo_Selector_SelectedIndexChanged(System::Object ^ aS, System::EventArgs ^ aE)
    {
        switch (mCombo_Selector->SelectedIndex)
        {
        case 0:
            mSource = false;

            mNum_A->Value = mC->GetDstA();
            mNum_B->Value = mC->GetDstB();
            mNum_C->Value = mC->GetDstC();
            mNum_D->Value = mC->GetDstD();
            break;

        case 1:
            mSource = true ;

            mNum_A->Value = mC->GetSrcA();
            mNum_B->Value = mC->GetSrcB();
            mNum_C->Value = mC->GetSrcC();
            mNum_D->Value = mC->GetSrcD();
            break;

        default: assert(false);
        }

        Pictures_Update();
    }

    // ===== NumericUpDown ==================================================

    System::Void Radar_View::mNum_A_ValueChanged(System::Object ^ aS, System::EventArgs ^ aE) { unsigned char lA = static_cast<unsigned char>(mNum_A->Value); mSource ? mC->SetSrcA(lA) : mC->SetDstA(lA); Pictures_Update(); }
    System::Void Radar_View::mNum_B_ValueChanged(System::Object ^ aS, System::EventArgs ^ aE) { unsigned char lB = static_cast<unsigned char>(mNum_B->Value); mSource ? mC->SetSrcB(lB) : mC->SetDstB(lB); Pictures_Update(); }
    System::Void Radar_View::mNum_C_ValueChanged(System::Object ^ aS, System::EventArgs ^ aE) { unsigned char lC = static_cast<unsigned char>(mNum_C->Value); mSource ? mC->SetSrcC(lC) : mC->SetDstC(lC); Pictures_Update(); }
    System::Void Radar_View::mNum_D_ValueChanged(System::Object ^ aS, System::EventArgs ^ aE) { unsigned char lD = static_cast<unsigned char>(mNum_D->Value); mSource ? mC->SetSrcD(lD) : mC->SetDstD(lD); Pictures_Update(); }

    System::Void Radar_View::mNum_Fade_ValueChanged(System::Object ^ aS, System::EventArgs ^ aE) { mC->SetFade(static_cast<unsigned char>(mNum_Fade->Value)); }
    System::Void Radar_View::mNum_Gain_ValueChanged(System::Object ^ aS, System::EventArgs ^ aE) { mC->SetGain(static_cast<unsigned char>(mNum_Gain->Value)); }

    // ===== PictureBox =====================================================

    System::Void Radar_View::mPict_AB_MouseClick(System::Object ^ aS, System::Windows::Forms::MouseEventArgs ^ aE)
    {
        mNum_A->Value = ( aE->Y * 256 ) / mPict_AB->Height;
        mNum_B->Value = ( aE->X * 256 ) / mPict_AB->Width ;
    }

    System::Void Radar_View::mPict_CD_MouseClick(System::Object ^ aS, System::Windows::Forms::MouseEventArgs ^ aE)
    {
        mNum_C->Value = (aE->Y * 256) / mPict_CD->Height;
        mNum_D->Value = (aE->X * 256) / mPict_CD->Width;
    }

    // ===== RadioButton ====================================================

    System::Void Radar_View::mBtn_And_CheckedChanged(System::Object ^ aS, System::EventArgs ^ aE)
    {
        assert(NULL != mC);

        if (mBtn_And->Checked)
        {
            mC->SetAnd();
        }
        else
        {
            mC->SetOr();
        }
    }

    // ===== Timer ==========================================================

    System::Void Radar_View::mTimer_Tick(System::Object ^ aS, System::EventArgs ^ aE)
    {
        assert(NULL != mC);

        mTick ++;

        unsigned int lTime_s  = mTick / 10;
        unsigned int lDecimal = mTick % 10;

        mLabel_Status->Text = lTime_s.ToString() + "." + lDecimal.ToString() + " s - " + gcnew System::String(mC->GetStatus()) + " ...";

        mC->Tick();

        Pictures_Update();

        mLabel_Status->Text = lTime_s.ToString() + "." + lDecimal.ToString() + " s - " + gcnew System::String(mC->GetStatus());
    }

}

// Static function declaration
/////////////////////////////////////////////////////////////////////////////

void Picture_Update(System::Windows::Forms::PictureBox ^ aPicture, const unsigned char * aData)
{
    assert(nullptr != aPicture);
    assert(NULL    != aData   );

    System::IntPtr lData(const_cast<unsigned char *>(aData));

    System::Drawing::Bitmap ^ lBitmap = gcnew System::Drawing::Bitmap(RADAR_SIZE, RADAR_SIZE, RADAR_SIZE * Radar_Controller::RGB_QTY, System::Drawing::Imaging::PixelFormat::Format24bppRgb, lData);

    aPicture->Image = lBitmap;
}
