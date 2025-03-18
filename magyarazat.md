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
TFile**     
