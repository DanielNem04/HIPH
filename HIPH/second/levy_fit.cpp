#include <iostream>
#include <vector>
#include <string>
#include "TH1.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TMath.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"
#include "TFile.h"
#include "TStyle.h"

// To calculate the 1D Levy-function
#include "levy_calc.h"

using namespace std;

TGraphErrors* gr;

double rmin;
double rmax;
const int NPARS = 3;
int NDF;

// Levy function to fit
double FitFunction(const double *x, const double *par)
{
	double r = x[0];
	double N = par[0];
    double R = par[1];
    double alpha = par[2];
    double Rcc = R*pow(2.,1. / alpha);
    return N*levy_calc(r/Rcc,Rcc,alpha);
}

// Chi-square error function
double MyChi2(const double *par)
{
    double chi2 = 0;
    NDF = 0;
    for(int ix=0;ix<gr->GetN();ix++)
    {
        double r = gr->GetX()[ix];
        if(r<rmin || r>rmax) continue;
        double exp = gr->GetY()[ix];
        double theor = FitFunction(&r,par);
        double err = gr->GetEY()[ix];
        if(err==0) continue;
        double chi = (exp-theor)/err;
        chi2 += chi*chi;
        NDF++;
    }
    NDF -= NPARS; // NDF = (number of fitted data points) - (number of fitting parameters)
    return chi2;
}

// Fitting and plotting one rho distribution
vector<double> levy_fit(const string& root_filename) {
    // Opening the root file and read the histogram
    TFile *file = new TFile(root_filename.c_str(), "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Failed to open the file: " << root_filename << std::endl;
        return {};
    }
    TH1F *hist = (TH1F*)file->Get("rho_hist");
   
    // Creating TGraphErrors object from the histogram data
    int nPoints = hist->GetNbinsX();
    gr = new TGraphErrors(nPoints);
    for (int i = 1; i <= nPoints; i++) {
        double x = hist->GetBinCenter(i);
        double y = hist->GetBinContent(i);
        double err = hist->GetBinError(i);
        gr->SetPoint(i-1, x, y);
        gr->SetPointError(i-1, 0, err);
    }

    // Set fitting and plotting boundaries
    rmin = 1.0;
    rmax = 200.0;
    double rminplot = 0.3;
    double rmaxplot = 1200.0;
    
    // Set up the minimization:
    ROOT::Minuit2::Minuit2Minimizer min ( ROOT::Minuit2::kCombined ); // Choose method upon creation between: kMigrad, kSimplex, kCombined, kScan, kFumili
    
    min.SetMaxFunctionCalls(1000000);
    min.SetMaxIterations(100000);
    min.SetTolerance(0.001);
    min.SetPrintLevel(2);
    
    ROOT::Math::Functor f(&MyChi2,NPARS);
    min.SetFunction(f);
    
    min.SetLimitedVariable(0,"N",      1.0 ,0.01, 0.4, 1.0);
    min.SetLimitedVariable(1,"R",      8.0 ,0.01, 3.0, 15.0);
    min.SetLimitedVariable(2,"alpha",  1.1 ,0.01, 0.8, 1.8);
    
    // Actual minimization
    min.Minimize();
    const double *par = min.X();
    const double *err = min.Errors();
    double chi2 = MyChi2(par);
    
    // Plotting the results
    /* Plot the results of the fit. Show the fitted parameters and their errors on the plot with TLatex) */
  
    
    return {par[0], err[0], par[1], err[1], par[2], err[2]};
}

int main() {
    // D_rho histo file names with centralities
    vector<string> root_files = {/* fill with calculated histograms */};

    vector<double> N_part = {/* fill with the N_part of the events that histos was created from */};
    
    vector<double> N_vals, N_errs, R_vals, R_errs, alpha_vals, alpha_errs, chi2_probs;
    
    // Iterating through the histograms and perform the fitting on all of them
    for (size_t i = 0; i < root_files.size(); i++) {
        vector<double> params = levy_fit(root_files[i]);
        if (params.empty()) continue;
        
        N_vals.push_back(params[0]);
        N_errs.push_back(params[1]);
        R_vals.push_back(params[2]);
        R_errs.push_back(params[3]);
        alpha_vals.push_back(params[4]);
        alpha_errs.push_back(params[5]);
    }
    
    // Plotting the parameters
    /* Plot the fitted parameters with their errors in function of <N_part> */
    
    return 0;
}

