#pragma once
#include "eboardinfo.h"
#include "smartcard.h"
namespace eBoard { 

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::IO::Ports;
	
	//using namespace System::Windows::Documents;
	/// <summary>
	/// Form1 摘要
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  maintab;
	protected: 
	private: System::Windows::Forms::TabPage^  page1;
	private: System::Windows::Forms::TabPage^  page2;
	private: System::Windows::Forms::TabPage^  page3;
	private: System::Windows::Forms::RichTextBox^  verinfo;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Button^  Btn1;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	private: System::IO::Ports::SerialPort^  serialPort1;

	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label1;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::GroupBox^  groupBox4;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button6;

	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::ComboBox^  comboBox3;

	private: System::Windows::Forms::ComboBox^  comboBox4;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  button9;












	private: System::ComponentModel::IContainer^  components;



	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->maintab = (gcnew System::Windows::Forms::TabControl());
			this->page1 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox4 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox3 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->Btn1 = (gcnew System::Windows::Forms::Button());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->page2 = (gcnew System::Windows::Forms::TabPage());
			this->page3 = (gcnew System::Windows::Forms::TabPage());
			this->verinfo = (gcnew System::Windows::Forms::RichTextBox());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->maintab->SuspendLayout();
			this->page1->SuspendLayout();
			this->groupBox4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer2))->BeginInit();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->page3->SuspendLayout();
			this->SuspendLayout();
			// 
			// maintab
			// 
			this->maintab->Controls->Add(this->page1);
			this->maintab->Controls->Add(this->page2);
			this->maintab->Controls->Add(this->page3);
			this->maintab->Dock = System::Windows::Forms::DockStyle::Fill;
			this->maintab->Location = System::Drawing::Point(0, 0);
			this->maintab->Margin = System::Windows::Forms::Padding(0);
			this->maintab->Name = L"maintab";
			this->maintab->SelectedIndex = 0;
			this->maintab->Size = System::Drawing::Size(584, 361);
			this->maintab->TabIndex = 0;
			this->maintab->Selected += gcnew System::Windows::Forms::TabControlEventHandler(this, &Form1::maintab_Selected);
			this->maintab->Click += gcnew System::EventHandler(this, &Form1::maintab_Click);
			// 
			// page1
			// 
			this->page1->Controls->Add(this->groupBox4);
			this->page1->Controls->Add(this->groupBox3);
			this->page1->Controls->Add(this->groupBox2);
			this->page1->Controls->Add(this->groupBox1);
			this->page1->Controls->Add(this->richTextBox1);
			this->page1->Location = System::Drawing::Point(4, 22);
			this->page1->Name = L"page1";
			this->page1->Padding = System::Windows::Forms::Padding(3);
			this->page1->Size = System::Drawing::Size(576, 335);
			this->page1->TabIndex = 0;
			this->page1->Text = L"老化板";
			this->page1->UseVisualStyleBackColor = true;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->splitContainer2);
			this->groupBox4->Dock = System::Windows::Forms::DockStyle::Left;
			this->groupBox4->Location = System::Drawing::Point(316, 3);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(136, 173);
			this->groupBox4->TabIndex = 9;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"常用";
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(3, 17);
			this->splitContainer2->Name = L"splitContainer2";
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->button9);
			this->splitContainer2->Panel1->Controls->Add(this->button7);
			this->splitContainer2->Panel1->Controls->Add(this->button6);
			this->splitContainer2->Size = System::Drawing::Size(130, 153);
			this->splitContainer2->SplitterDistance = 62;
			this->splitContainer2->TabIndex = 0;
			// 
			// button9
			// 
			this->button9->Dock = System::Windows::Forms::DockStyle::Top;
			this->button9->Location = System::Drawing::Point(0, 46);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(62, 23);
			this->button9->TabIndex = 2;
			this->button9->Text = L"str2hex";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->Click += gcnew System::EventHandler(this, &Form1::button9_Click);
			// 
			// button7
			// 
			this->button7->Dock = System::Windows::Forms::DockStyle::Top;
			this->button7->Location = System::Drawing::Point(0, 23);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(62, 23);
			this->button7->TabIndex = 1;
			this->button7->Text = L"hex2str";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &Form1::button7_Click);
			// 
			// button6
			// 
			this->button6->Dock = System::Windows::Forms::DockStyle::Top;
			this->button6->Location = System::Drawing::Point(0, 0);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(62, 23);
			this->button6->TabIndex = 0;
			this->button6->Text = L"清缓存";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->splitContainer1);
			this->groupBox3->Dock = System::Windows::Forms::DockStyle::Left;
			this->groupBox3->Location = System::Drawing::Point(3, 3);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(313, 173);
			this->groupBox3->TabIndex = 8;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"指令集";
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(3, 17);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->listBox1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->button5);
			this->splitContainer1->Panel2->Controls->Add(this->button4);
			this->splitContainer1->Panel2->Controls->Add(this->button3);
			this->splitContainer1->Panel2->Controls->Add(this->button2);
			this->splitContainer1->Size = System::Drawing::Size(307, 153);
			this->splitContainer1->SplitterDistance = 235;
			this->splitContainer1->TabIndex = 0;
			// 
			// listBox1
			// 
			this->listBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listBox1->FormattingEnabled = true;
			this->listBox1->ItemHeight = 12;
			this->listBox1->Location = System::Drawing::Point(0, 0);
			this->listBox1->Name = L"listBox1";
			this->listBox1->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->listBox1->Size = System::Drawing::Size(235, 153);
			this->listBox1->TabIndex = 0;
			this->listBox1->Click += gcnew System::EventHandler(this, &Form1::listBox1_Click);
			this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listBox1_SelectedIndexChanged);
			// 
			// button5
			// 
			this->button5->Dock = System::Windows::Forms::DockStyle::Top;
			this->button5->Location = System::Drawing::Point(0, 69);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(68, 23);
			this->button5->TabIndex = 3;
			this->button5->Text = L"打开指令";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// button4
			// 
			this->button4->Dock = System::Windows::Forms::DockStyle::Top;
			this->button4->Location = System::Drawing::Point(0, 46);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(68, 23);
			this->button4->TabIndex = 2;
			this->button4->Text = L"保存指令";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// button3
			// 
			this->button3->Dock = System::Windows::Forms::DockStyle::Top;
			this->button3->Location = System::Drawing::Point(0, 23);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(68, 23);
			this->button3->TabIndex = 1;
			this->button3->Text = L"删除指令";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// button2
			// 
			this->button2->Dock = System::Windows::Forms::DockStyle::Top;
			this->button2->Location = System::Drawing::Point(0, 0);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(68, 23);
			this->button2->TabIndex = 0;
			this->button2->Text = L"增加指令";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->textBox1);
			this->groupBox2->Controls->Add(this->label1);
			this->groupBox2->Controls->Add(this->button1);
			this->groupBox2->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->groupBox2->Location = System::Drawing::Point(3, 176);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(570, 44);
			this->groupBox2->TabIndex = 7;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"发送命令";
			this->groupBox2->Enter += gcnew System::EventHandler(this, &Form1::groupBox2_Enter);
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->textBox1->Dock = System::Windows::Forms::DockStyle::Left;
			this->textBox1->Location = System::Drawing::Point(3, 17);
			this->textBox1->Margin = System::Windows::Forms::Padding(0);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(453, 21);
			this->textBox1->TabIndex = 4;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			// 
			// label1
			// 
			this->label1->Dock = System::Windows::Forms::DockStyle::Right;
			this->label1->Location = System::Drawing::Point(456, 17);
			this->label1->Margin = System::Windows::Forms::Padding(0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(36, 24);
			this->label1->TabIndex = 5;
			this->label1->Text = L"0";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label1->Click += gcnew System::EventHandler(this, &Form1::label1_Click);
			// 
			// button1
			// 
			this->button1->AutoSize = true;
			this->button1->Dock = System::Windows::Forms::DockStyle::Right;
			this->button1->Location = System::Drawing::Point(492, 17);
			this->button1->Margin = System::Windows::Forms::Padding(0);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 24);
			this->button1->TabIndex = 3;
			this->button1->Text = L"发送";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->comboBox4);
			this->groupBox1->Controls->Add(this->comboBox3);
			this->groupBox1->Controls->Add(this->comboBox2);
			this->groupBox1->Controls->Add(this->Btn1);
			this->groupBox1->Controls->Add(this->comboBox1);
			this->groupBox1->Location = System::Drawing::Point(460, 3);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(110, 173);
			this->groupBox1->TabIndex = 6;
			this->groupBox1->TabStop = false;
			this->groupBox1->Enter += gcnew System::EventHandler(this, &Form1::groupBox1_Enter);
			// 
			// label2
			// 
			this->label2->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->label2->Location = System::Drawing::Point(3, 142);
			this->label2->Margin = System::Windows::Forms::Padding(3);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(104, 28);
			this->label2->TabIndex = 6;
			this->label2->Text = L"8Bit数据位       1Bit停止位";
			this->label2->Visible = false;
			// 
			// comboBox4
			// 
			this->comboBox4->Dock = System::Windows::Forms::DockStyle::Top;
			this->comboBox4->FormattingEnabled = true;
			this->comboBox4->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"NONE", L"ODD", L"EVEN"});
			this->comboBox4->Location = System::Drawing::Point(3, 116);
			this->comboBox4->Name = L"comboBox4";
			this->comboBox4->Size = System::Drawing::Size(104, 20);
			this->comboBox4->TabIndex = 5;
			this->comboBox4->Text = L"NONE";
			this->comboBox4->Visible = false;
			// 
			// comboBox3
			// 
			this->comboBox3->Dock = System::Windows::Forms::DockStyle::Top;
			this->comboBox3->FormattingEnabled = true;
			this->comboBox3->Location = System::Drawing::Point(3, 96);
			this->comboBox3->Name = L"comboBox3";
			this->comboBox3->Size = System::Drawing::Size(104, 20);
			this->comboBox3->TabIndex = 3;
			// 
			// comboBox2
			// 
			this->comboBox2->Dock = System::Windows::Forms::DockStyle::Top;
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Location = System::Drawing::Point(3, 76);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(104, 20);
			this->comboBox2->TabIndex = 2;
			this->comboBox2->Text = L"CCID";
			// 
			// Btn1
			// 
			this->Btn1->Dock = System::Windows::Forms::DockStyle::Top;
			this->Btn1->Location = System::Drawing::Point(3, 37);
			this->Btn1->Name = L"Btn1";
			this->Btn1->Size = System::Drawing::Size(104, 39);
			this->Btn1->TabIndex = 0;
			this->Btn1->Text = L"连接eBoard";
			this->Btn1->UseVisualStyleBackColor = true;
			this->Btn1->Click += gcnew System::EventHandler(this, &Form1::Btn1_Click);
			// 
			// comboBox1
			// 
			this->comboBox1->Dock = System::Windows::Forms::DockStyle::Top;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"USB", L"UART"});
			this->comboBox1->Location = System::Drawing::Point(3, 17);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(104, 20);
			this->comboBox1->TabIndex = 1;
			this->comboBox1->Text = L"USB";
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_TextChanged);
			// 
			// richTextBox1
			// 
			this->richTextBox1->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->richTextBox1->Location = System::Drawing::Point(3, 220);
			this->richTextBox1->Margin = System::Windows::Forms::Padding(0);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(570, 112);
			this->richTextBox1->TabIndex = 2;
			this->richTextBox1->Text = L"";
			this->richTextBox1->TextChanged += gcnew System::EventHandler(this, &Form1::richTextBox1_TextChanged);
			// 
			// page2
			// 
			this->page2->Location = System::Drawing::Point(4, 22);
			this->page2->Name = L"page2";
			this->page2->Padding = System::Windows::Forms::Padding(3);
			this->page2->Size = System::Drawing::Size(576, 335);
			this->page2->TabIndex = 1;
			this->page2->Text = L"Python";
			this->page2->UseVisualStyleBackColor = true;
			// 
			// page3
			// 
			this->page3->Controls->Add(this->verinfo);
			this->page3->Location = System::Drawing::Point(4, 22);
			this->page3->Name = L"page3";
			this->page3->Padding = System::Windows::Forms::Padding(3);
			this->page3->Size = System::Drawing::Size(576, 335);
			this->page3->TabIndex = 2;
			this->page3->Text = L"版本";
			this->page3->UseVisualStyleBackColor = true;
			// 
			// verinfo
			// 
			this->verinfo->Dock = System::Windows::Forms::DockStyle::Fill;
			this->verinfo->Location = System::Drawing::Point(3, 3);
			this->verinfo->Margin = System::Windows::Forms::Padding(0);
			this->verinfo->Name = L"verinfo";
			this->verinfo->Size = System::Drawing::Size(570, 329);
			this->verinfo->TabIndex = 0;
			this->verinfo->Text = L"";
			this->verinfo->Click += gcnew System::EventHandler(this, &Form1::verinfo_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(584, 361);
			this->Controls->Add(this->maintab);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"eBoard";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->maintab->ResumeLayout(false);
			this->page1->ResumeLayout(false);
			this->groupBox4->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer2))->EndInit();
			this->splitContainer2->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->page3->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void maintab_Click(System::Object^  sender, System::EventArgs^  e) {
				try
				{
					eboardinfo^ ebinfo = gcnew eboardinfo();
					verinfo->Text = ebinfo->eboardinfo_version_string;
				}
				catch(...)
				{
					
				}
			 }

private: System::Void verinfo_Click(System::Object^  sender, System::EventArgs^  e) {
			 
		 }
private: System::Void maintab_Selected(System::Object^  sender, System::Windows::Forms::TabControlEventArgs^  e) {

		 }
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }


private: System::Void groupBox1_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void groupBox2_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 //增加指令
				listBox1->Items->Add(textBox1->Text); 	
				listBox1->SelectedIndex = listBox1->Items->Count - 1;
			 }
			 catch(...)
			 {
			 }
		 }
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
			//打开指令集文档
			 try
			 {
				 int i;
				 array<String^>^cmdlines;
				 openFileDialog1->InitialDirectory = ".//";
				 openFileDialog1->Filter =  "txt files (*.txt)|*.txt|All files (*.*)|*.*";
				 openFileDialog1->FilterIndex =1;
				 openFileDialog1->ShowDialog();
				 cmdlines = File::ReadAllLines(openFileDialog1->FileName);
				 listBox1->Items->Clear();
				 for (i=0;i<cmdlines->Length;i++)
				 {
					listBox1->Items->Add(cmdlines[i]);
				 }
				 
				 
				 
				 
			 }
			 catch(...)
			 {}
		 }

private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 try
		 {
			String^ cmdstr=textBox1->Text;
			cmdstr=cmdstr->Replace(" ","");
			cmdstr=cmdstr->Replace("	","");
			int cmdlen = cmdstr->Length;
			label1->Text = Convert::ToString(cmdlen/2.0);			
		 }
		 catch(...)
		 {
			
		 }
		 
		 }
private: System::Void listBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 //删除指令
			 try
			 {
				listBox1->Items->Remove(listBox1->SelectedItem);
			 }
			 catch(...){}
			 
		 }
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
			 //保存指令集到指定文件
			 try
			 {
				 int i;
				 array<String^>^cmdlines=gcnew array<String^>(listBox1->Items->Count);
				 saveFileDialog1->InitialDirectory = ".//";
				 saveFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
				 saveFileDialog1->FilterIndex = 1;
				 saveFileDialog1->ShowDialog();
				 for(i=0;i<listBox1->Items->Count;i++)
				 {
					cmdlines[i]=listBox1->Items[i]->ToString();
				 }
				 File::WriteAllLines(saveFileDialog1->FileName,cmdlines);
				 
				 
			 }
			 catch(...)
			 {

			 }
		 }
private: System::Void listBox1_Click(System::Object^  sender, System::EventArgs^  e) {
			 //更新指令集中的指令到指令发送栏
			 try
			 {
				 // 命名行中的“#”是注释的作用，不加载到命令行中去。
				 String^ cmdstr;
				 cmdstr=listBox1->SelectedItem->ToString();
				 if(cmdstr->IndexOf("#")>=0)
					cmdstr=cmdstr->Substring(0,cmdstr->IndexOf("#"));
				 textBox1->Text = cmdstr;
			 }
			 catch(...){}
		 }
private: System::Void richTextBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void comboBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 int i;
				 scard = gcnew smartcard();
				 if(comboBox1->Text=="USB")
				 {
					 comboBox4->Visible = false;
					 label2->Visible = false;
					 button1->Text = "发送";
					 comboBox2->Items->Clear();
					 comboBox2->Items->Add("CCID");
					 comboBox2->SelectedIndex=0;
					 scard->GetReaders();
					 if(scard->ReaderCnt>0)
					 {
						 comboBox3->Items->Clear();
						 for(i=0;i<scard->ReaderCnt;i++)
						 {
							comboBox3->Items->Add(scard->ReaderName[i]);
							ShowMessage(scard->ReaderName[i]);
						 }						 
						 comboBox3->SelectedIndex = 0;
						
					 }
					 else
					 {
						 ShowMessage("没有可用的卡机");
						 comboBox3->Items->Clear();
					 }
				 }
				 else if(comboBox1->Text=="UART")
				 {
					 comboBox4->Visible = true;
					 label2->Visible = true;
					 comboBox2->Items->Clear();
					 comboBox2->Items->Add("9600");
					 comboBox2->Items->Add("57600");
					 comboBox2->Items->Add("74880");
					 comboBox2->Items->Add("115200");
					 //comboBox2->Items->Add("输入任意波特率");
					 comboBox2->SelectedIndex=3;
								 
					 comboBox3->Items->Clear();
					 comboBox3->Items->AddRange(serialPort1->GetPortNames());
					 comboBox3->SelectedIndex = 0;
					 
					 comboBox4->SelectedIndex = 0;


				 }
				 else
				 {
					 comboBox2->Text = "";
					 comboBox2->Items->Clear();
					 //comboBox3->Visible = false;
				 }
			 }
		 catch(...){}
		 }
private: System::Void Btn1_Click(System::Object^  sender, System::EventArgs^  e) {
			 //连接eBoard
			 try
			 {
				 int ret;
				 if(Btn1->Text=="连接eBoard")
				 {			 
					 if(comboBox1->Text=="USB")
					 {
						 ret=scard->ConnectReader(comboBox3->SelectedIndex);
						 if(ret==0)
						 {
							ShowMessage("连接 "+comboBox3->Text+" 成功");
							Btn1->Text = "断开eBoard";
						 }
						 else
						 {
							ShowMessage("连接 "+comboBox3->Text+" 失败");
						 }						 
					 }
					 else //uart
					 {
						 if(serialPort1->IsOpen) 
							 ShowMessage("串口已被占用");
						 else
						 {
							serialPort1->PortName = comboBox3->Text;
							serialPort1->BaudRate = Convert::ToInt32(comboBox2->Text);
							serialPort1->DataBits = 8;
							serialPort1->StopBits = StopBits::One;
							if(comboBox4->Text=="EVEN")
								serialPort1->Parity   = Parity::Even;
							else if(comboBox4->Text=="ODD")
								serialPort1->Parity   = Parity::Odd;
							else 
								serialPort1->Parity	  = Parity::None;
							serialPort1->Open();
							if(serialPort1->IsOpen) {
								ShowMessage("串口打开成功:1Bit停止位,8Bit位宽,校验"+comboBox4->Text);
								Btn1->Text = "断开eBoard";
							}
							else 
								ShowMessage("串口打开失败");							
						 }							 						 						 
					 }
				 }
				 else
				 {
					 Btn1->Text = "连接eBoard";
					 if(comboBox1->Text=="USB")
					 {
						if(scard->DisconnectReader()==0)
						{
							ShowMessage("关闭 "+comboBox3->Text+" 成功");
						}
						else
						{
							ShowMessage("关闭 "+comboBox3->Text+" 失败");
						}
						
					 }
					 else
					 {
						serialPort1->Close();
						ShowMessage("关闭串口成功");
					 }				
				 }

			 }
			 catch(...){}

		 }
//*******************************YPF Start**********************
private: smartcard^ scard;
private: int cmdcount;
private: void ShowMessage(String ^str)
		 {
			 try
			 {
				 richTextBox1->Text += cmdcount.ToString();
				 richTextBox1->Text += (">> "+str);
				 richTextBox1->Text += "\n";
				 richTextBox1->Select();
				 richTextBox1->Select(richTextBox1->TextLength,0);
				 richTextBox1->ScrollToCaret();
				 //richTextBox1->Focus();
				 cmdcount++;
			 }
			 catch(...)
			 {

			 }
		 }


private: String^ Hex2Ascii(String^ str){//"65"->"e"
			 int i;
			 String^ retstr;
			 String^ instr = str->Replace(" ","")->Replace("	","");
			 try
			 {
				 if(instr->Length%2) 
					 throw i;
				 else
				 {
					 for(i=0;i<instr->Length;i=i+2)
					 {
						 retstr += Convert::ToChar(Convert::ToByte(instr->Substring(i,2),16));
					 }
					 return retstr;
				 }
			 }
			 catch(...)
			 {
				 retstr = "";
				 return retstr;
			 }			 
		 }

private: String^ Ascii2Hex(String^ str){//"e"->"65"	 
			int i;
			String^ retstr;
			String^ instr = str->Replace("	","")->Replace("	","");
			try
			{
				retstr="";
				for(i=0;i<instr->Length;i++)
				{
					retstr += Convert::ToString(Convert::ToByte(Convert::ToChar(instr->Substring(i,1))),16);					
				} 
			}
			catch(...)
			{
				retstr="";
			}
			return retstr;
		 }
//*******************************YPF End**********************

private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 //发送指令
			 String^ sendstr;
			 String^ recvstr;
			 int serialtemp;
			 try
			 {
				 //array
				 if(Btn1->Text=="断开eBoard")
				 {

					 if(comboBox1->Text=="USB")
					 {//USB发送
						 sendstr=textBox1->Text->Replace(" ","")->Replace("	","");						 
						 if(scard->TransmitReader(sendstr,recvstr)==0)
						 {//发送成功
							ShowMessage(recvstr);
						 }
						 else
						 {//发送失败
							ShowMessage("CCID 发送失败");
						 }						 
					 }
					 else
					 {//Uart发送，格式怎么设置，需要进一步确认
						if(textBox1->Text->Replace(" ","")->Replace("	","")->Length%2)
							ShowMessage("请输入正确的发送数据");
						else
						{
							sendstr=textBox1->Text->Replace(" ","")->Replace("	","");
							serialPort1->Write(Hex2Ascii(sendstr));
							ShowMessage("串口发送: "+sendstr);
							serialtemp=serialPort1->ReadByte();
							Sleep(20);
							recvstr+=Convert::ToString(serialtemp,16);
							ShowMessage("串口接受: "+recvstr->ToUpper());
						}

					 }
				 }
				 else
				 {
					 ShowMessage("请连接eBoard");
				 }
			 }
			 catch(...)
			 {}
		 }
private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 cmdcount=0;
				 richTextBox1->Clear();
			 }
			 catch(...)
			 {}
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				cmdcount=0;
			 }
			 catch(...)
			 {

			 }
		 }
private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {
			 //hex2str
			 try
			 {
				 textBox1->Text = Hex2Ascii(textBox1->Text);
			 }
			 catch(...)
			 {}
		 }
private: System::Void button9_Click(System::Object^  sender, System::EventArgs^  e) {
			 //str2hex
			 try
			 {
				 textBox1->Text = Ascii2Hex(textBox1->Text);
			 }
			 catch(...)
			 {}
		 }
private: System::Void serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
		 }
private: System::Void serialPort1_DataReceived_1(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
		 }
};
}

