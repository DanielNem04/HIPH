# Az $D(\rho)$ eloszlás elkészítésének lépése.



## 1. Először a már használt 'z-gg2my59a-1.root'-tree-jének brancheit viszgáltam meg.

A benne lévő treek előhívása: 
```cpp
root -l
TFile *f = TFile::open('z-gg2my59a-1.root');
f->ls();
```
Ekkor a visszakapott üzenet:
```cpp
TFile**		/Users/nemeskeridaniel/Downloads/hiph/rho_distribution/z-gg2my59a-1.root	
 TFile*		/Users/nemeskeridaniel/Downloads/hiph/rho_distribution/z-gg2my59a-1.root	
  KEY: TTree	teposevent0;2	particles [current cycle]
  KEY: TTree	teposevent0;1	particles [backup cycle]
  KEY: TTree	teposhead0;1	header
  KEY: TTree	teposhead;1	header
  KEY: TTree	teposevent;1	particles
```
A már használt 'tree_reader.C' fileban a   
```cpp
TFile* inputfile = new TFile("z-gg2my59a-1.root");
  TTree* tree = (TTree*)inputfile->Get("teposevent");
```
szerepelt így ennek a Brancheit listázva:
```cpp
TTree *tree = (TTree*)_file0->Get("teposevent");
tree->Print();
******************************************************************************
*Tree    :teposevent: particles                                              *
*Entries :       30 : Total =       128114853 bytes  File  Size =   58300599 *
*        :          : Tree compression factor =   2.20                       *
******************************************************************************
*Br    0 :np        : np/I                                                   *
*Entries :       30 : Total  Size=        745 bytes  File Size  =        272 *
*Baskets :        2 : Basket Size=      36624 bytes  Compression=   1.00     *
*............................................................................*
*Br    1 :bim       : bim/F                                                  *
*Entries :       30 : Total  Size=        751 bytes  File Size  =        229 *
*Baskets :        2 : Basket Size=      36624 bytes  Compression=   1.20     *
*............................................................................*
*Br    2 :nev       : nev/I                                                  *
*Entries :       30 : Total  Size=        751 bytes  File Size  =        202 *
*Baskets :        2 : Basket Size=      36624 bytes  Compression=   1.36     *
*............................................................................*
*Br    3 :npt       : npt/I                                                  *
*Entries :       30 : Total  Size=        751 bytes  File Size  =        274 *
*Baskets :        2 : Basket Size=      36624 bytes  Compression=   1.00     *
*............................................................................*
.
.
.
*............................................................................*
*Br   17 :px        : px[np]/F                                               *
*Entries :       30 : Total  Size=    9150232 bytes  File Size  =    8437455 *
*Baskets :       17 : Basket Size=    4904448 bytes  Compression=   1.08     *
*............................................................................*
*Br   18 :py        : py[np]/F                                               *
*Entries :       30 : Total  Size=    9150232 bytes  File Size  =    8436430 *
*Baskets :       17 : Basket Size=    4904448 bytes  Compression=   1.08     *
*............................................................................*
*Br   19 :pz        : pz[np]/F                                               *
*Entries :       30 : Total  Size=    9150232 bytes  File Size  =    8535669 *
*Baskets :       17 : Basket Size=    4904448 bytes  Compression=   1.07     *
*............................................................................*
*Br   20 :e         : e[np]/F                                                *
*Entries :       30 : Total  Size=    9150211 bytes  File Size  =    2592451 *
*Baskets :       17 : Basket Size=    4904448 bytes  Compression=   3.53     *
*............................................................................*
*Br   21 :x         : x[np]/F                                                *
*Entries :       30 : Total  Size=    9150211 bytes  File Size  =    6135651 *
*Baskets :       17 : Basket Size=    4904448 bytes  Compression=   1.49     *
*............................................................................*
*Br   22 :y         : y[np]/F                                                *
*Entries :       30 : Total  Size=    9150211 bytes  File Size  =    6133079 *
*Baskets :       17 : Basket Size=    4904448 bytes  Compression=   1.49     *
*............................................................................*
*Br   23 :z         : z[np]/F                                                *
*Entries :       30 : Total  Size=    9150211 bytes  File Size  =    6154666 *
*Baskets :       17 : Basket Size=    4904448 bytes  Compression=   1.49     *
*............................................................................*
*Br   24 :t         : t[np]/F                                                *
*Entries :       30 : Total  Size=    9150211 bytes  File Size  =    6365666 *
*Baskets :       17 : Basket Size=    4904448 bytes  Compression=   1.44     *
*............................................................................*
*Br   25 :id        : id[np]/I                                               *
*Entries :       30 : Total  Size=    9150225 bytes  File Size  =    2152534 *
*Baskets :       17 : Basket Size=    4904448 bytes  Compression=   4.25     *
*............................................................................*
.
.
.
*............................................................................*
*Br   29 :jor       : jor[np]/I                                              *
*Entries :       30 : Total  Size=    9150246 bytes  File Size  =     409662 *
*Baskets :       17 : Basket Size=    4904448 bytes  Compression=  22.33     *
*............................................................................*
```
Így megtaláltam hogy x, y, z (a labor vonatkoztatási rendszerében) is megtaláltahó a px, py, pz-hez hasonlóan a tree-ben.

## A kiszámolandó mennyiség(ek)

Az áhított mennyiség: $|\rho|=\displaystyle \sqrt{(\rho_{out}^{LCMS})^2+(\rho_{side}^{LCMS})^2+(\rho_{long}^{LCMS})^2}.$

A szereplő mennyiségek: 

$$\rho_{out}^{LCMS} = r_x \cdot \dfrac{K_x}{\displaystyle \sqrt{K_x^2 + K_y^2}} + r_y \cdot \dfrac{K_y}{\displaystyle \sqrt{K_x^2 + K_y^2}}  - \dfrac{\displaystyle \sqrt{K_x^2 + K_y^2}}{K_0^2 -K_z^2}\cdot \left(K_0t-K_z r_z\right)$$

$$ \rho_{side}^{LCMS} = -r_x \cdot \dfrac{K_y}{\displaystyle \sqrt{K_x^2 + K_y^2}}+r_y\cdot  \dfrac{K_x}{\displaystyle \sqrt{K_x^2 + K_y^2}}$$

$$  \rho_{long}^{LCMS} = \dfrac{K_0 r_z -K_zt}{\displaystyle \sqrt{K_0^2 -K_z^2}}$$

$$K^{\mu}= \dfrac{1}{2} (p_1^{\mu}+p_2^{\mu} ), \quad r_x=\sqrt{x_1^2-x_2^2} $$

Ezután ezen függvényeket dekralártam cpp-ban!

```cpp
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

double compute_r_x(const Particle& p1, const Particle& p2) {
    return std::sqrt(p1.x * p1.x - p2.x * p2.x);
}

double compute_rho_LCMS(const Particle& p1, const Particle& p2) {
    FourVector K = compute_K(p1, p2);
    double r_x = compute_r_x(p1, p2);
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
```


Ezután maga a számoló ciklus(ok):

```cpp
  int nEntries = tree->GetEntries();
  
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

        std::vector<double> rho_values;

        // Compute rho_LCMS for filtered particles
        int selectedSize = selectedParticles.size();
        for (int i = 0; i < selectedSize - 1; i++) {
            for (int j = i + 1; j < selectedSize; j++) {
                rho_values.push_back(compute_rho_LCMS(selectedParticles[i], selectedParticles[j]));
            }
        }

        std::cerr << "Computed " << rho_values.size() << " rho values." << std::endl;
    }
```
