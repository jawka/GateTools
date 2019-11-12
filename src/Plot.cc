#include "Plot.hh"


Plot::Plot(std::string scanner_mode_string)
{
	scanner_mode = scanner_mode_string;
}



Plot::~Plot()
{
	scanner_mode = " ";
}




void Plot::plot_complex_profile_analysis ()
{

	// Read root file
	std::string f_name = dir_desktop + ieee_file + scanner_mode + root_ext;
	std::cout << "ROOT file name: " << f_name << std::endl;


	gStyle->SetOptStat(kFALSE);

	TCanvas *c1 = new TCanvas("Complex plot","Complex plot",2800,1400);
	TFile *root_file;

	root_file = new TFile(f_name.c_str(), "READ");

	//DOSE PROFILE
	std::string sdose = "Dose profile";
	std::string registered_over_time = "Registered annihilations in coincidence - integrated over time";
	std::string original_over_time = "Original place of gammas annihilation - integrated over time";

	c1->cd(1);

	gStyle->SetOptStat(kFALSE);
	gPad->SetGridx();
	gPad->SetGridy();

	TH1F *registered_integrated = (TH1F*)root_file->Get(registered_over_time.c_str());

	registered_integrated->SetTitle("Analysis of the coincidences integrated over time");

	registered_integrated->GetYaxis()->SetTitle("Number of coincidences");
	registered_integrated->GetYaxis()->SetLabelFont(62);
	registered_integrated->GetYaxis()->SetTitleFont(62);
	registered_integrated->GetYaxis()->SetRangeUser(0, 2*registered_integrated->GetMaximum());
	registered_integrated->GetXaxis()->SetTitle("Phantom depth [mm]");	
	registered_integrated->GetXaxis()->SetLabelFont(62);
	registered_integrated->GetXaxis()->SetTitleFont(62);
	registered_integrated->SetLineWidth(1);
	registered_integrated->Draw();
	


	c1->Update();


	TH1F *original_integrated = (TH1F*)root_file->Get(original_over_time.c_str());

	original_integrated->SetLineWidth(1);
	original_integrated->SetLineColor(kRed);
	original_integrated->Draw("same");



	TH1F *dose = (TH1F*)root_file->Get(sdose.c_str());

	Float_t rightmax = 1.1*dose->GetMaximum();
	Float_t scale = gPad->GetUymax()/rightmax;

	std::cout << rightmax << std::endl;
	std::cout << scale << std::endl;

	dose->SetLineWidth(1);
	dose->SetLineColor(kViolet);

	dose->Scale(scale);
	dose->Draw("same hist");

	c1->Update();

	TGaxis *axis = new TGaxis(gPad->GetUxmax(), gPad->GetUymin(), gPad->GetUxmax(), gPad->GetUymax(), 0, rightmax, 510, "+L");
	axis->SetLineColor(kViolet);
	axis->SetLabelColor(kViolet);
	axis->SetTitle("Dose [a.u]");
	axis->SetTitleColor(kViolet);
	axis->Draw();

	c1->Update();


	// FITTING
	TF1 *fit_proximal_recon = new TF1("proximal_recon","[0]*TMath::Erf((x-[1])/[2])+[3]", atof(std::getenv("PRM_PH_MIN")) + 25., atof(std::getenv("PRM_PH_MIN")) + 65.); 
	fit_proximal_recon->SetParameters(150., -100, 8., 150);
	
	TF1 *fit_distal_recon = new TF1("distal_recon","[0]*TMath::Erf((x-[1])/[2])+[3]", atof(std::getenv("PRM_BETA_RANGE")) - 10., atof(std::getenv("PRM_BETA_RANGE")) + 40.); 
	fit_distal_recon->SetParameters(-150, 30, 4., 150);
	std::cout << "Max of the reconstructed z profile histogram: " << registered_integrated->GetMaximum() << std::endl;

	TF1 *fit_distal_mc = new TF1("distal_mc","[0]*TMath::Erf((x-[1])/[2])+[3]", atof(std::getenv("PRM_BETA_RANGE")) - 5., atof(std::getenv("PRM_BETA_RANGE")) + 40.); 
	fit_distal_mc->SetParameters(-300, 30, 4., 300);
	std::cout << "Max of the original (MC) z profile histogram: " << original_integrated->GetMaximum() << std::endl;


	fit_proximal_recon->SetLineColor(5);
	fit_proximal_recon->SetLineWidth(2);
	fit_proximal_recon->SetLineStyle(1);
	fit_distal_recon->SetLineColor(11);
	fit_distal_recon->SetLineWidth(2);
	fit_distal_recon->SetLineStyle(1);
	fit_distal_mc->SetLineColor(12);
	fit_distal_mc->SetLineWidth(2);
	fit_distal_mc->SetLineStyle(1);

	registered_integrated->Fit(fit_proximal_recon,"FEBR");
	registered_integrated->Fit(fit_distal_recon,"FEBR+");
	original_integrated->Fit(fit_distal_mc,"FEBR+");

	std::cout << std::endl << "########## FITTING RESULTS ##########" << std::endl << std::endl;
	std::cout << "Proximal original fit:" << std::endl;
	std::cout << "\tx: " << (atof(std::getenv("PRM_PH_MIN")) + 50.) << " mm" << std::endl;
	std::cout << "Distal original fit:" << std::endl;
	std::cout << "\tx: " << fit_distal_mc->GetParameter(1) << " mm" << std::endl;
	std::cout << "Proximal pseudo-reconstructed fit:" << std::endl;
	std::cout << "\tx: " << fit_proximal_recon->GetParameter(1) << " mm" << std::endl;
	std::cout << "Distal pseudo-reconstructed fit:" << std::endl;
	std::cout << "\tx: " << fit_distal_recon->GetParameter(1) << " mm" << std::endl;

	std::stringstream original_dw50;
	original_dw50 << "Original delta w50%: " << TMath::Abs((atof(std::getenv("PRM_PH_MIN")) + 50.) - fit_distal_mc->GetParameter(1)) << " mm";
	std::stringstream reconstructed_dw50;
	reconstructed_dw50 << "Pseudo-reconstructed delta w50%: " << TMath::Abs(fit_proximal_recon->GetParameter(1) - fit_distal_recon->GetParameter(1)) << " mm";
	
	std::cout << std::endl << original_dw50.str() << std::endl;
	std::cout << reconstructed_dw50.str() << std::endl;
	std::cout << std::endl << "########## END ##########" << std::endl << std::endl;
	
	c1->Update();

	// LEGEND

	TLegend *legend = new TLegend(0.5, 0.6, 0.87, 0.87);
	legend->SetTextSize(0.02);
	legend->SetFillColor(0);
	legend->AddEntry(registered_integrated,"Pseudo-reconstructed annihilation point profile","lp");
	legend->AddEntry(original_integrated,"Original annihilation point profile","lp");
	legend->AddEntry(dose,"Dose","lp");
	legend->AddEntry(fit_distal_mc,"Erf fit to the original MC data - DISTAL fall-off","lp");
	legend->AddEntry(fit_proximal_recon,"Erf fit to the pseudo-reconstructed data - PROXIMAL fall-off","lp");
	legend->AddEntry(fit_distal_recon,"Erf fit to the pseudo-reconstructed data - DISTAL fall-off","lp");
	legend->AddEntry((TObject*)0, original_dw50.str().c_str(), "");
	legend->AddEntry((TObject*)0, reconstructed_dw50.str().c_str(), "");
	legend->Draw();

	c1->Update();

	f_name = dir_desktop + complex_analysis_file + scanner_mode + ".pdf";
	c1->SaveAs(f_name.c_str());

	c1->Close();




}








void Plot::plot_IEEE_integrated ()
{

	// Read root file
	std::string f_name = dir_desktop + ieee_file + scanner_mode + root_ext;
	std::cout << "ROOT file name: " << f_name << std::endl;


	gStyle->SetOptStat(kFALSE);

	TCanvas *c1 = new TCanvas("IEEE MIC","IEEE MIC",2800,1400);
	TFile *root_file;

	root_file = new TFile(f_name.c_str(), "READ");

	//DOSE PROFILE
	std::string sdose = "Dose profile";
	std::string original_over_time = "Original place of gammas annihilation - integrated over time";

	c1->cd(1);

	gStyle->SetOptStat(kFALSE);
	gPad->SetGridx();
	gPad->SetGridy();

	TH1F *original_over_time_h = (TH1F*)root_file->Get(original_over_time.c_str());

	original_over_time_h->SetTitle("Analysis of the coincidences - integrated over time");
	original_over_time_h->SetTitle("");

	original_over_time_h->GetYaxis()->SetTitle("Number of coincidences");
	original_over_time_h->GetYaxis()->SetLabelFont(62);
	original_over_time_h->GetYaxis()->SetTitleFont(62);
	original_over_time_h->GetYaxis()->SetLabelSize(0.05);
	original_over_time_h->GetYaxis()->SetTitleSize(0.05);
	original_over_time_h->GetYaxis()->SetRangeUser(0, 400);
	original_over_time_h->GetYaxis()->SetNdivisions(205);

	original_over_time_h->GetXaxis()->SetTitle("Phantom depth [mm]");	
	original_over_time_h->GetXaxis()->SetLabelFont(62);
	original_over_time_h->GetXaxis()->SetTitleFont(62);
	original_over_time_h->GetXaxis()->SetLabelSize(0.05);
	original_over_time_h->GetXaxis()->SetTitleSize(0.05);
	original_over_time_h->GetXaxis()->SetNdivisions(203);


	original_over_time_h->GetXaxis()->SetRangeUser(-150., 100.);


	original_over_time_h->SetLineWidth(2);
	original_over_time_h->Draw();
	
	c1->Update();


	TH1F *dose = (TH1F*)root_file->Get(sdose.c_str());

	Float_t rightmax = 1.1;
	Float_t scale = gPad->GetUymax()/(1.1*dose->GetMaximum());

	std::cout << rightmax << std::endl;
	std::cout << scale << std::endl;

	dose->SetLineWidth(2);
	dose->SetLineColor(kRed);

	dose->Scale(scale);
	dose->Draw("same hist");
	
	c1->Update();

	TGaxis *axis = new TGaxis(gPad->GetUxmax(), gPad->GetUymin(), gPad->GetUxmax(), gPad->GetUymax(), 0, rightmax, 510, "+L");
	axis->SetLineColor(kRed);
	axis->SetLabelColor(kRed);
	axis->SetTitle("Dose [a.u]");
	axis->SetTitleColor(kRed);
	axis->SetTitleSize(0.05);
	axis->SetLabelSize(0.05);
	axis->SetNdivisions(205);
	axis->Draw();

	c1->Update();


	// LEGEND
	Int_t coincidences_number = 330000;
	std::stringstream all_coincidences; 
	all_coincidences << "All coincidences occured within the phantom: " << coincidences_number;
	std::stringstream plastic_coincidences;
	plastic_coincidences << "Coincidences registered by the system (true fraction): " << original_over_time_h->GetEntries();
	
	TLegend *legend = new TLegend(0.15, 0.65, 0.5, 0.85);
	legend->SetTextSize(0.04);
	legend->AddEntry(original_over_time_h,"Annihilation point","lp");
	legend->AddEntry(dose,"Dose","lp");
	legend->Draw();

	c1->Update();

	f_name = dir_desktop + ieee_file_integrated + scanner_mode + ".pdf";
	c1->SaveAs(f_name.c_str());

	c1->Close();




}







void Plot::plot_IEEE_interspill ()
{

	// Read root file
	std::string f_name = dir_desktop + ieee_file + scanner_mode + root_ext;
	std::cout << "ROOT file name: " << f_name << std::endl;


	gStyle->SetOptStat(kFALSE);

	TCanvas *c1 = new TCanvas("IEEE MIC","IEEE MIC",2800,1400);
	TFile *root_file;

	root_file = new TFile(f_name.c_str(), "READ");

	//DOSE PROFILE
	std::string sdose = "Dose profile";
	std::string original_interspill = "Original place of gammas annihilation - interspill phase";

	c1->cd(1);

	gPad->SetGridx();
	gPad->SetGridy();

	TH1F *original_interspill_h = (TH1F*)root_file->Get(original_interspill.c_str());

	original_interspill_h->SetTitle("Analysis of the coincidences - 5 minutes after the irradiation");
	original_interspill_h->SetTitle("");

	original_interspill_h->GetYaxis()->SetTitle("Number of coincidences");
	original_interspill_h->GetYaxis()->SetLabelFont(62);
	original_interspill_h->GetYaxis()->SetTitleFont(62);
	original_interspill_h->GetYaxis()->SetRangeUser(0, 2*original_interspill_h->GetMaximum());
	original_interspill_h->GetYaxis()->SetLabelSize(0.05);
	original_interspill_h->GetYaxis()->SetTitleSize(0.05);
	original_interspill_h->GetYaxis()->SetNdivisions(205);

	original_interspill_h->GetXaxis()->SetTitle("Phantom depth [mm]");	
	original_interspill_h->GetXaxis()->SetLabelFont(62);
	original_interspill_h->GetXaxis()->SetTitleFont(62);
	original_interspill_h->GetXaxis()->SetLabelSize(0.05);
	original_interspill_h->GetXaxis()->SetTitleSize(0.05);
	original_interspill_h->GetXaxis()->SetNdivisions(203);


	original_interspill_h->GetXaxis()->SetRangeUser(-150., 100.);


	original_interspill_h->SetLineWidth(2);
	original_interspill_h->Draw();
	
	c1->Update();


	TH1F *dose = (TH1F*)root_file->Get(sdose.c_str());


	Float_t rightmax = 1.1;
	Float_t scale = gPad->GetUymax()/(1.1*dose->GetMaximum());

	std::cout << rightmax << std::endl;
	std::cout << scale << std::endl;

	dose->SetLineWidth(2);
	dose->SetLineColor(kRed);

	dose->Scale(scale);
	dose->Draw("same hist");
	
	c1->Update();

	TGaxis *axis = new TGaxis(gPad->GetUxmax(), gPad->GetUymin(), gPad->GetUxmax(), gPad->GetUymax(), 0, rightmax, 510, "+L");
	axis->SetLineColor(kRed);
	axis->SetLabelColor(kRed);
	axis->SetTitle("Dose [a.u]");
	axis->SetTitleColor(kRed);
	axis->SetLabelSize(0.05);
	axis->SetTitleSize(0.05);
	axis->SetNdivisions(205);	
	axis->Draw();

	c1->Update();


	// LEGEND
	Int_t coincidences_number = 330000;
	std::stringstream all_coincidences; 
	all_coincidences << "All coincidences occurede within the phantom: " << coincidences_number;
	std::stringstream plastic_coincidences;
	plastic_coincidences << "Coincidences registered by the system (true fraction): " << original_interspill_h->GetEntries();
	
	TLegend *legend = new TLegend(0.15, 0.65, 0.5, 0.85);
	legend->SetTextSize(0.04);
	legend->AddEntry(original_interspill_h,"Annihilation point","lp");
	legend->AddEntry(dose,"Dose","lp");
	legend->Draw();

	c1->Update();

	f_name = dir_desktop + ieee_file_interspill + scanner_mode + ".pdf";
	c1->SaveAs(f_name.c_str());

	c1->Close();




}








