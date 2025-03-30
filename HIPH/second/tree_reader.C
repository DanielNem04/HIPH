#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <iostream>
#include <cmath>
#include <tuple>

const int npMax = 84000; // Maximum number of particles in an event
// declaration of the function and structures for calc


struct FourVector {
    double t, x, y, z;
};

struct Particle {
    double m, px, py, pz, x, y, z, t;
    int id, ist;
};

FourVector compute_K(const Particle& p1, const Particle& p2) {
    return {0.5 * (std::sqrt(p1.m * p1.m + p1.px * p1.px + p1.py * p1.py + p1.pz * p1.pz) +
                   std::sqrt(p2.m * p2.m + p2.px * p2.px + p2.py * p2.py + p2.pz * p2.pz)),
            0.5 * (p1.px + p2.px),
            0.5 * (p1.py + p2.py),
            0.5 * (p1.pz + p2.pz)};
}


double compute_rho_LCMS(const Particle& p1, const Particle& p2) {
    FourVector K = compute_K(p1, p2);
    double r_x = p1.x-p2.x;
    double r_y = p1.y - p2.y;
    double r_z = p1.z - p2.z;
    double t = p1.t - p2.t;  

    double K_perp = std::sqrt(K.x * K.x + K.y * K.y);
    double K_long = std::sqrt(K.t * K.t - K.z * K.z);
    
    double rho_out = (r_x * K.x / K_perp) + (r_y * K.y / K_perp) - (K_perp / K_long) * (K.t * t - K.z * r_z);
    double rho_side = (-r_x * K.y / K_perp) + (r_y * K.x / K_perp);
    double rho_long = (K.t * r_z - K.z * t) / K_long;

    return std::sqrt(rho_out * rho_out + rho_side * rho_side + rho_long * rho_long);
}



void tree_reader()
{
  // Open ROOT file and obtain tree
  TFile* inputfile = new TFile("z-gg2my59a-1.root");
  TTree* tree = (TTree*)inputfile->Get("teposevent");
  if(tree) cerr << "Opened root file." << endl;
  else { cerr << "Could not open root file." << endl; return ; }

  // Declare variables for reading the tree
  int np;
  int id[npMax], ist[npMax]; 
  float mass[npMax], px[npMax], py[npMax], pz[npMax], x[npMax], y[npMax], z[npMax], t[npMax];

  // Set up branch addresses
  tree->SetBranchAddress("np", &np); // number of particles in the event
  tree->SetBranchAddress("id", &id); // particle ID
  tree->SetBranchAddress("e", &mass); // mass (stored as "e" in the tree)
  tree->SetBranchAddress("px", &px); // momentum, x direction
  tree->SetBranchAddress("py", &py); // momentum, y direction
  tree->SetBranchAddress("pz", &pz); // momentum, z direction
  // simularly x, y and z coordinates
  tree->SetBranchAddress("x", &x);
  tree->SetBranchAddress("y", &y);
  tree->SetBranchAddress("z", &z);
  tree->SetBranchAddress("t", &t);
  tree->SetBranchAddress("ist", &ist); 

  int nEntries = tree->GetEntries();
  TH1D* h_rho = new TH1D("h_rho", "Corrected D(rho);#rho [fm];D(#rho)", 50, 0, 10.0);

  std::vector<double> rho_values;

    for (int iEvent = 0; iEvent < nEntries; iEvent++) {
        tree->GetEntry(iEvent);
        std::cerr << "Processing event " << iEvent << " with " << np << " particles." << std::endl;

        std::vector<Particle> selectedParticles;

        for (int i = 0; i < np; i++) {
            if (id[i] == 120 && ist[i] == 0) {  
                double pT = std::sqrt(px[i] * px[i] + py[i] * py[i]);
                double eta = 0.5 * std::log((std::sqrt(px[i] * px[i] + py[i] * py[i] + pz[i] * pz[i]) + pz[i]) /
                                            (std::sqrt(px[i] * px[i] + py[i] * py[i] + pz[i] * pz[i]) - pz[i]));

                if (pT > 0.15 && pT < 1.0 && std::abs(eta) < 1.0) {  
                    selectedParticles.push_back({mass[i], px[i], py[i], pz[i], x[i], y[i], z[i], t[i], id[i], ist[i]});
                }
            }
        }

        int selectedSize = selectedParticles.size();
        for (int i = 0; i < selectedSize - 1; i++) {
            for (int j = i + 1; j < selectedSize; j++) {
                rho_values.push_back(compute_rho_LCMS(selectedParticles[i], selectedParticles[j]));
            }
        }

        std::cerr << "Computed " << rho_values.size() << " rho values." << std::endl;
    }
    
    for (double rho : rho_values) {
        if (rho > 0.99) {  // Csak a 0.99 fm-nél nagyobb értékek kerüljenek be
        double weight = 1.0 / (4 * M_PI * rho * rho);
        h_rho->Fill(rho, weight);
        }
    }

    TCanvas* c1 = new TCanvas("c1", "D(rho) Corrected", 800, 600);
    gStyle->SetOptStat(0);
    
    c1->SetLogx();  
    c1->SetLogy();  

    h_rho->SetLineColor(kBlue);
    h_rho->SetLineWidth(2);
    h_rho->Draw();
    
    c1->SaveAs("D_rho_plot.png");
}