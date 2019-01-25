
// Author   KMS - Martin Dubois, ing.
// Product  OpenNet
// File     Samples/Radar/Radar_View.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== Samples/Radar ======================================================
#include "Radar_Controller.h"

namespace Radar {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Radar_View
	/// </summary>
	public ref class Radar_View : public System::Windows::Forms::Form
	{
	public:

        Radar_View(void);

	protected:

        ~Radar_View();

	private:

        void Pictures_Update();

        Radar_Controller * mC     ;
        bool               mSource;
        unsigned int       mTick  ;

    private: System::Windows::Forms::Button^  mBtn_Stop;
    private: System::Windows::Forms::Button^  mBtn_Start;
    private: System::Windows::Forms::RadioButton^  mBtn_And;
    private: System::Windows::Forms::RadioButton^  mBtn_Or;
    private: System::Windows::Forms::NumericUpDown^  mNum_Gain;
    private: System::Windows::Forms::Label^  mLabel_Gain;
    private: System::Windows::Forms::NumericUpDown^  mNum_D;









    private: System::Windows::Forms::NumericUpDown^  mNum_C;
    private: System::Windows::Forms::PictureBox^  mPict_CD;


    private: System::Windows::Forms::PictureBox^  mPict_AB;

    private: System::Windows::Forms::NumericUpDown^  mNum_B;

    private: System::Windows::Forms::NumericUpDown^  mNum_A;

    private: System::Windows::Forms::Timer^  mTimer;
    private: System::Windows::Forms::StatusStrip^  statusStrip1;
    private: System::Windows::Forms::ToolStripStatusLabel^  mLabel_Status;
    private: System::Windows::Forms::NumericUpDown^  mNum_Fade;

    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::ComboBox^  mCombo_Selector;

    private: System::ComponentModel::IContainer^  components;

		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            this->mBtn_Stop = (gcnew System::Windows::Forms::Button());
            this->mBtn_Start = (gcnew System::Windows::Forms::Button());
            this->mBtn_And = (gcnew System::Windows::Forms::RadioButton());
            this->mBtn_Or = (gcnew System::Windows::Forms::RadioButton());
            this->mNum_Gain = (gcnew System::Windows::Forms::NumericUpDown());
            this->mLabel_Gain = (gcnew System::Windows::Forms::Label());
            this->mNum_D = (gcnew System::Windows::Forms::NumericUpDown());
            this->mNum_C = (gcnew System::Windows::Forms::NumericUpDown());
            this->mPict_CD = (gcnew System::Windows::Forms::PictureBox());
            this->mPict_AB = (gcnew System::Windows::Forms::PictureBox());
            this->mNum_B = (gcnew System::Windows::Forms::NumericUpDown());
            this->mNum_A = (gcnew System::Windows::Forms::NumericUpDown());
            this->mTimer = (gcnew System::Windows::Forms::Timer(this->components));
            this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
            this->mLabel_Status = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->mNum_Fade = (gcnew System::Windows::Forms::NumericUpDown());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->mCombo_Selector = (gcnew System::Windows::Forms::ComboBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mNum_Gain))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mNum_D))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mNum_C))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mPict_CD))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mPict_AB))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mNum_B))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mNum_A))->BeginInit();
            this->statusStrip1->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mNum_Fade))->BeginInit();
            this->SuspendLayout();
            // 
            // mBtn_Stop
            // 
            this->mBtn_Stop->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->mBtn_Stop->Location = System::Drawing::Point(1487, 13);
            this->mBtn_Stop->Name = L"mBtn_Stop";
            this->mBtn_Stop->Size = System::Drawing::Size(75, 41);
            this->mBtn_Stop->TabIndex = 0;
            this->mBtn_Stop->Text = L"Stop";
            this->mBtn_Stop->UseVisualStyleBackColor = true;
            this->mBtn_Stop->Click += gcnew System::EventHandler(this, &Radar_View::mBtn_Stop_Click);
            // 
            // mBtn_Start
            // 
            this->mBtn_Start->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->mBtn_Start->Location = System::Drawing::Point(1406, 13);
            this->mBtn_Start->Name = L"mBtn_Start";
            this->mBtn_Start->Size = System::Drawing::Size(75, 41);
            this->mBtn_Start->TabIndex = 1;
            this->mBtn_Start->Text = L"Start";
            this->mBtn_Start->UseVisualStyleBackColor = true;
            this->mBtn_Start->Click += gcnew System::EventHandler(this, &Radar_View::mBtn_Start_Click);
            // 
            // mBtn_And
            // 
            this->mBtn_And->AutoSize = true;
            this->mBtn_And->Location = System::Drawing::Point(12, 12);
            this->mBtn_And->Name = L"mBtn_And";
            this->mBtn_And->Size = System::Drawing::Size(73, 29);
            this->mBtn_And->TabIndex = 2;
            this->mBtn_And->Text = L"And";
            this->mBtn_And->UseVisualStyleBackColor = true;
            this->mBtn_And->CheckedChanged += gcnew System::EventHandler(this, &Radar_View::mBtn_And_CheckedChanged);
            // 
            // mBtn_Or
            // 
            this->mBtn_Or->AutoSize = true;
            this->mBtn_Or->Checked = true;
            this->mBtn_Or->Location = System::Drawing::Point(92, 12);
            this->mBtn_Or->Name = L"mBtn_Or";
            this->mBtn_Or->Size = System::Drawing::Size(59, 29);
            this->mBtn_Or->TabIndex = 3;
            this->mBtn_Or->TabStop = true;
            this->mBtn_Or->Text = L"Or";
            this->mBtn_Or->UseVisualStyleBackColor = true;
            // 
            // mNum_Gain
            // 
            this->mNum_Gain->Location = System::Drawing::Point(260, 13);
            this->mNum_Gain->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 127, 0, 0, 0 });
            this->mNum_Gain->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
            this->mNum_Gain->Name = L"mNum_Gain";
            this->mNum_Gain->Size = System::Drawing::Size(74, 29);
            this->mNum_Gain->TabIndex = 4;
            this->mNum_Gain->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
            this->mNum_Gain->ValueChanged += gcnew System::EventHandler(this, &Radar_View::mNum_Gain_ValueChanged);
            // 
            // mLabel_Gain
            // 
            this->mLabel_Gain->AutoSize = true;
            this->mLabel_Gain->Location = System::Drawing::Point(201, 13);
            this->mLabel_Gain->Name = L"mLabel_Gain";
            this->mLabel_Gain->Size = System::Drawing::Size(53, 25);
            this->mLabel_Gain->TabIndex = 5;
            this->mLabel_Gain->Text = L"Gain";
            // 
            // mNum_D
            // 
            this->mNum_D->Location = System::Drawing::Point(921, 72);
            this->mNum_D->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 255, 0, 0, 0 });
            this->mNum_D->Name = L"mNum_D";
            this->mNum_D->Size = System::Drawing::Size(120, 29);
            this->mNum_D->TabIndex = 5;
            this->mNum_D->ValueChanged += gcnew System::EventHandler(this, &Radar_View::mNum_D_ValueChanged);
            // 
            // mNum_C
            // 
            this->mNum_C->Location = System::Drawing::Point(794, 72);
            this->mNum_C->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 255, 0, 0, 0 });
            this->mNum_C->Name = L"mNum_C";
            this->mNum_C->Size = System::Drawing::Size(120, 29);
            this->mNum_C->TabIndex = 4;
            this->mNum_C->ValueChanged += gcnew System::EventHandler(this, &Radar_View::mNum_C_ValueChanged);
            // 
            // mPict_CD
            // 
            this->mPict_CD->Location = System::Drawing::Point(794, 109);
            this->mPict_CD->Name = L"mPict_CD";
            this->mPict_CD->Size = System::Drawing::Size(768, 768);
            this->mPict_CD->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->mPict_CD->TabIndex = 3;
            this->mPict_CD->TabStop = false;
            this->mPict_CD->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Radar_View::mPict_CD_MouseClick);
            // 
            // mPict_AB
            // 
            this->mPict_AB->Location = System::Drawing::Point(12, 109);
            this->mPict_AB->Name = L"mPict_AB";
            this->mPict_AB->Size = System::Drawing::Size(768, 768);
            this->mPict_AB->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->mPict_AB->TabIndex = 2;
            this->mPict_AB->TabStop = false;
            this->mPict_AB->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Radar_View::mPict_AB_MouseClick);
            // 
            // mNum_B
            // 
            this->mNum_B->Location = System::Drawing::Point(139, 72);
            this->mNum_B->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 255, 0, 0, 0 });
            this->mNum_B->Name = L"mNum_B";
            this->mNum_B->Size = System::Drawing::Size(120, 29);
            this->mNum_B->TabIndex = 1;
            this->mNum_B->ValueChanged += gcnew System::EventHandler(this, &Radar_View::mNum_B_ValueChanged);
            // 
            // mNum_A
            // 
            this->mNum_A->Location = System::Drawing::Point(12, 72);
            this->mNum_A->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 255, 0, 0, 0 });
            this->mNum_A->Name = L"mNum_A";
            this->mNum_A->Size = System::Drawing::Size(120, 29);
            this->mNum_A->TabIndex = 0;
            this->mNum_A->ValueChanged += gcnew System::EventHandler(this, &Radar_View::mNum_A_ValueChanged);
            // 
            // mTimer
            // 
            this->mTimer->Tick += gcnew System::EventHandler(this, &Radar_View::mTimer_Tick);
            // 
            // statusStrip1
            // 
            this->statusStrip1->ImageScalingSize = System::Drawing::Size(28, 28);
            this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->mLabel_Status });
            this->statusStrip1->Location = System::Drawing::Point(0, 892);
            this->statusStrip1->Name = L"statusStrip1";
            this->statusStrip1->Size = System::Drawing::Size(1574, 35);
            this->statusStrip1->TabIndex = 8;
            this->statusStrip1->Text = L"statusStrip1";
            // 
            // mLabel_Status
            // 
            this->mLabel_Status->Name = L"mLabel_Status";
            this->mLabel_Status->Size = System::Drawing::Size(89, 30);
            this->mLabel_Status->Text = L"Stopped";
            // 
            // mNum_Fade
            // 
            this->mNum_Fade->Location = System::Drawing::Point(459, 12);
            this->mNum_Fade->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 127, 0, 0, 0 });
            this->mNum_Fade->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
            this->mNum_Fade->Name = L"mNum_Fade";
            this->mNum_Fade->Size = System::Drawing::Size(79, 29);
            this->mNum_Fade->TabIndex = 9;
            this->mNum_Fade->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
            this->mNum_Fade->ValueChanged += gcnew System::EventHandler(this, &Radar_View::mNum_Fade_ValueChanged);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(389, 13);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(57, 25);
            this->label1->TabIndex = 10;
            this->label1->Text = L"Fade";
            // 
            // mCombo_Selector
            // 
            this->mCombo_Selector->FormattingEnabled = true;
            this->mCombo_Selector->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Destination", L"Source" });
            this->mCombo_Selector->Location = System::Drawing::Point(631, 13);
            this->mCombo_Selector->Name = L"mCombo_Selector";
            this->mCombo_Selector->Size = System::Drawing::Size(151, 32);
            this->mCombo_Selector->TabIndex = 11;
            this->mCombo_Selector->Text = L"Destination";
            this->mCombo_Selector->SelectedIndexChanged += gcnew System::EventHandler(this, &Radar_View::mCombo_Selector_SelectedIndexChanged);
            // 
            // Radar_View
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(11, 24);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1574, 927);
            this->Controls->Add(this->mNum_D);
            this->Controls->Add(this->mCombo_Selector);
            this->Controls->Add(this->mNum_C);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->mPict_CD);
            this->Controls->Add(this->mNum_Fade);
            this->Controls->Add(this->mPict_AB);
            this->Controls->Add(this->mNum_B);
            this->Controls->Add(this->statusStrip1);
            this->Controls->Add(this->mNum_A);
            this->Controls->Add(this->mLabel_Gain);
            this->Controls->Add(this->mNum_Gain);
            this->Controls->Add(this->mBtn_Or);
            this->Controls->Add(this->mBtn_And);
            this->Controls->Add(this->mBtn_Start);
            this->Controls->Add(this->mBtn_Stop);
            this->Name = L"Radar_View";
            this->Text = L"Radar";
            this->Load += gcnew System::EventHandler(this, &Radar_View::Radar_View_Load);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mNum_Gain))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mNum_D))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mNum_C))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mPict_CD))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mPict_AB))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mNum_B))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mNum_A))->EndInit();
            this->statusStrip1->ResumeLayout(false);
            this->statusStrip1->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mNum_Fade))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void Radar_View_Load(System::Object^  sender, System::EventArgs^  e);
    private: System::Void mNum_A_ValueChanged(System::Object^  sender, System::EventArgs^  e);
    private: System::Void mNum_B_ValueChanged(System::Object^  sender, System::EventArgs^  e);
    private: System::Void mNum_C_ValueChanged(System::Object^  sender, System::EventArgs^  e);
    private: System::Void mNum_D_ValueChanged(System::Object^  sender, System::EventArgs^  e);
    private: System::Void mBtn_And_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
    private: System::Void mNum_Gain_ValueChanged(System::Object^  sender, System::EventArgs^  e);
    private: System::Void mBtn_Start_Click(System::Object^  sender, System::EventArgs^  e);
    private: System::Void mBtn_Stop_Click(System::Object^  sender, System::EventArgs^  e);
    private: System::Void mTimer_Tick(System::Object^  sender, System::EventArgs^  e);
    private: System::Void mNum_Fade_ValueChanged(System::Object^  sender, System::EventArgs^  e);
    private: System::Void mCombo_Selector_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
    private: System::Void mPict_CD_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
    private: System::Void mPict_AB_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);

};
}
