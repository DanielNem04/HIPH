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

$$\rho_{out}^{LCMS} = x \cdot \dfrac{K_x}{\displaystyle \sqrt{K_x^2 + K_y^2}} + y \cdot \dfrac{K_y}{\displaystyle \sqrt{K_x^2 + K_y^2}}  - \dfrac{\displaystyle \sqrt{K_x^2 + K_y^2}}{K_0^2 -K_z^2}\cdot \left(K_0t-K_zz\right)$$

$$ \rho_{side}^{LCMS} = -x \cdot \dfrac{K_y}{\displaystyle \sqrt{K_x^2 + K_y^2}}+y\cdot  \dfrac{K_x}{\displaystyle \sqrt{K_x^2 + K_y^2}}$$

$$  \rho_{long}^{LCMS} = \dfrac{K_0 z -K_zt}{\displaystyle \sqrt{K_0^2 -K_z^2}}$$

$$K^{\mu}= \dfrac{1}{2} (p_1^{\mu}+p_2^{\mu} ) $$

Ezután ezen függvényeket dekralártam cpp-ban!

