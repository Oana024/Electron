#include <bits/stdc++.h>
#include <winbgim.h>
#include <graphics.h>

using namespace std;

DWORD ecranx=GetSystemMetrics(SM_CXSCREEN);
DWORD ecrany=GetSystemMetrics(SM_CYSCREEN);

char piese[101][101], informatii[1001][1001];
int marimeTextMare, marimeTextMica, grosimeLinie=5, hitbox=15;
int nrButoane=5, nrPiese=0, nrLinii=0;
int dimensiunePiesa=50, dimensiunePiesaStandard=100, piesaSelectata=1, orientare=1, valoare=10;
int rezolutiex=ecranx, rezolutiey=ecrany;
int sfarsit;
int dimensiunemax=400, dimensiunemin=50;
int page=1, page2=1, paginaSelectata=2, randuriPePagina=15;
int numarProiecte=6, numarMaximProiecte=8;
int seLeaga=0, sejoaca=1, seSchimba=0, inSaveLoad=0, estePunct=0;
int mutat=0, sterge=0, amStersLeg=0;

struct apasat
{
    char nume[101];
    int x1, y1, x2, y2;
};

apasat buton[101];

struct punct
{
    int x, y, viz;
};

struct piesa
{
    int numarLegaturi;
    punct Leg[101];
};

struct lst
{
    int TipNod;
    int x, y, marime;
    int orientare;
    int sters=0;
    int valoare;
    piesa P;
};

struct Graf
{
    int NumarNoduri;
    lst Noduri[101];
};

Graf Circuit;

struct legaturi
{
    int x1, y1, x2, y2;
    int piesa1, piesa2;
    int mijlocx, mijlocy;
    int legatura1, legatura2;
    int sters;
};
legaturi linii[1001];

void magie() ///Asta e stupid, dar merge!
{
    readimagefile("background.jpg", 0, 0, 0, 0);
}

void clearall()
{
    clearmouseclick(WM_LBUTTONDOWN);
    clearmouseclick(WM_RBUTTONDOWN);
    clearmouseclick(WM_LBUTTONUP);
}

void citirePiese() ///Citeste cate tipuri de piesa avem si care sunt acestea.
{
    FILE *fisier_piese=fopen("piese.txt", "r");
    fscanf(fisier_piese, "%d", &nrPiese);
    for(int i=1; i<=nrPiese; i++)
        fscanf(fisier_piese, "%s", &piese[i]);
    fclose(fisier_piese);
}

void butoane()
{
    FILE *numebuton=fopen("butoane.txt", "r");
    fscanf(numebuton, "%d", &nrButoane);
    for(int i=1; i<=nrButoane; i++)
    {
        fscanf(numebuton, "%s", &buton[i].nume);
        buton[i].x1=27*rezolutiex/32;
        buton[i].y1=(i*2-1)*2*rezolutiey/32;
        buton[i].x2=31*rezolutiex/32;
        buton[i].y2=i*4*rezolutiey/32;
    }
    fclose(numebuton);
}

int verificaButon(int x, int y)
{
    int indice=(y/(2*rezolutiey/32)+1);
    if(indice%2==1 || indice/2 > nrButoane || x < buton[1].x1 || x > buton[1].x2)
        return -1;
    return indice/2;
}

bool avem_legaturi()
{
    for(int i=1; i<=nrLinii; i++)
        if(linii[i].sters==0)
            return true;
    return false;
}

void linie(int xinitial, int yinitial, int xfinal, int yfinal,int careLinie)
{
    int ok;
    if(yinitial<yfinal)
        if(xinitial<xfinal)
            ok=1;
        else
            ok=0;
    else
        if(xinitial<xfinal)
            ok=0;
        else
            ok=1;

    if(ok==0)
    {
        linii[careLinie].mijlocx=xfinal;
        linii[careLinie].mijlocy=yinitial;
        line(xinitial, yinitial, xfinal, yinitial);
        line(xfinal, yinitial, xfinal, yfinal);
    }
    else
    {
        linii[careLinie].mijlocx=xinitial;
        linii[careLinie].mijlocy=yfinal;
        line(xinitial, yinitial, xinitial, yfinal);
        line(xinitial, yfinal, xfinal, yfinal);
    }
}

void patrat(int x1, int y1, int x2, int y2)
{
    line(x1, y1, x1, y2);
    line(x1, y2, x2, y2);
    line(x2, y2, x2, y1);
    line(x2, y1, x1, y1);
}

void desenarePiesa(int index, int x, int y, int marime, int orientare)
{
    char s[101];
    strcpy(s, piese[index]);
    strcat(s, "\\\\");
    s[strlen(s)-1]=orientare+'0';
    strcat(s, ".jpg");
    readimagefile(s, x-marime, y-marime, x+marime, y+marime);
}

void deseneaza()
{
    setactivepage(page);
    setvisualpage(1-page);
    page=1-page;
    clearviewport();

    int i;
    for(i=1; i<=Circuit.NumarNoduri; i++)
    {
        if(Circuit.Noduri[i].sters==0)
            desenarePiesa(Circuit.Noduri[i].TipNod, Circuit.Noduri[i].x, Circuit.Noduri[i].y, Circuit.Noduri[i].marime, Circuit.Noduri[i].orientare);
    }
    for(i=1; i<=nrLinii; i++)
    {
        if(linii[i].sters==0)
            linie(linii[i].x1, linii[i].y1, linii[i].x2, linii[i].y2, i);
    }
    line(26*rezolutiex/32, 0, 26*rezolutiex/32, rezolutiey);
    line(0, 4*rezolutiey/32, 26*rezolutiex/32, 4*rezolutiey/32);

    settextstyle(SANS_SERIF_FONT, 0, marimeTextMare);
    bgiout<<"ELECTRON";
    outstreamxy(13*rezolutiex/32-textwidth("ELECTRON")/2, 2*rezolutiey/32-textheight("ELECTRON")/2);
    settextstyle(SANS_SERIF_FONT, 0, marimeTextMica);

    for(i=1; i<=nrButoane; i++)
    {
        patrat(buton[i].x1, buton[i].y1, buton[i].x2, buton[i].y2);
        bgiout<<buton[i].nume;
        outstreamxy((buton[i].x1+buton[i].x2)/2-textwidth(buton[i].nume)/2, (buton[i].y1+buton[i].y2)/2-textheight(buton[i].nume)/2);
        if((i==4 && mutat==1) || (i==5 && sterge==1))
        {
            patrat(buton[i].x1+grosimeLinie+1, buton[i].y1+grosimeLinie+1, buton[i].x2-grosimeLinie-1, buton[i].y2-grosimeLinie-1);
            patrat(buton[i].x1+(grosimeLinie+1)*2, buton[i].y1+(grosimeLinie+1)*2, buton[i].x2-(grosimeLinie+1)*2, buton[i].y2-(grosimeLinie+1)*2);
        }
    }

    if(seLeaga==0)
        patrat(grosimeLinie, grosimeLinie, dimensiunePiesaStandard/2, dimensiunePiesaStandard/2);
    else
        bar(grosimeLinie, grosimeLinie, dimensiunePiesaStandard/2, dimensiunePiesaStandard/2);

    magie();
    setvisualpage(page);
}

void Deseneaza()///Nu stiu de ce , nu vreau sa stiu de ce , nu stiu de ce vreau sa stiu de ce , dar altfel nu merge
{
    deseneaza();
    deseneaza();
}

int Calcul(int xCurent, int RezolutieVeche, int RezolutieCurenta)
{
    int xFinal=xCurent*RezolutieCurenta/RezolutieVeche;
    return xFinal;
}

int Distanta_dintre_2_puncte(int x, int y)
{
    return sqrt((x*x)+(y*y));
}

int Find(piesa X, int x, int y)
{
    int i, distanta, distantaMinima=rezolutiex, NrLegatura=-1;
    for(i=1; i<=X.numarLegaturi; i++)
        if(!X.Leg[i].viz)
        {
            distanta=Distanta_dintre_2_puncte((x-X.Leg[i].x), (y-X.Leg[i].y));
            if(distanta<distantaMinima)
            {
                distantaMinima=distanta;
                NrLegatura=i;
            }
        }
    return NrLegatura;
}

void coordNoi(int x, int y, int &xnou, int &ynou, int orientare, int dimensiunePiesa)
{
    switch (orientare)
    {
        case 1:
            xnou=y;
            ynou=x;
            break;
        case 2:
            xnou=-x;
            ynou=-y;
            break;
        case 3:
            xnou=-y;
            ynou=-x;
    }
    xnou=(dimensiunePiesa*2.0)/100*xnou;
    ynou=(dimensiunePiesa*2.0)/100*ynou;
}

void actualizare(int piesaAleasa,int x,int y)
{
    int i;

    Circuit.Noduri[piesaAleasa].x=max(min(x, 26*rezolutiex/32-Circuit.Noduri[piesaAleasa].marime), Circuit.Noduri[piesaAleasa].marime);
    Circuit.Noduri[piesaAleasa].y=min(max(y, 4*rezolutiey/32+Circuit.Noduri[piesaAleasa].marime), rezolutiey-Circuit.Noduri[piesaAleasa].marime);

    char fisierPuncte[101];
    strcpy(fisierPuncte, "puncteLegatura\\");
    strcat(fisierPuncte, piese[Circuit.Noduri[piesaAleasa].TipNod]);
    strcat(fisierPuncte, ".txt");

    FILE *fisier=fopen(fisierPuncte, "r");
    fscanf(fisier, "%d", &Circuit.Noduri[piesaAleasa].P.numarLegaturi);
    for(i=1; i<=Circuit.Noduri[piesaAleasa].P.numarLegaturi; i++)
    {
        fscanf(fisier, "%d", &Circuit.Noduri[piesaAleasa].P.Leg[i].x);
        fscanf(fisier, "%d", &Circuit.Noduri[piesaAleasa].P.Leg[i].y);
        coordNoi(Circuit.Noduri[piesaAleasa].P.Leg[i].x, Circuit.Noduri[piesaAleasa].P.Leg[i].y, Circuit.Noduri[piesaAleasa].P.Leg[i].x,
                Circuit.Noduri[piesaAleasa].P.Leg[i].y, Circuit.Noduri[piesaAleasa].orientare-1, Circuit.Noduri[piesaAleasa].marime);
        Circuit.Noduri[piesaAleasa].P.Leg[i].x+=Circuit.Noduri[piesaAleasa].x;
        Circuit.Noduri[piesaAleasa].P.Leg[i].y+=Circuit.Noduri[piesaAleasa].y;
    }

    for(i=1; i<=nrLinii; i++)
    {
        if(linii[i].piesa1==piesaAleasa)
        {
            linii[i].x1=Circuit.Noduri[piesaAleasa].P.Leg[linii[i].legatura1].x;
            linii[i].y1=Circuit.Noduri[piesaAleasa].P.Leg[linii[i].legatura1].y;
        }
        if(linii[i].piesa2==piesaAleasa)
        {
            linii[i].x2=Circuit.Noduri[piesaAleasa].P.Leg[linii[i].legatura2].x;
            linii[i].y2=Circuit.Noduri[piesaAleasa].P.Leg[linii[i].legatura2].y;
        }
    }
    ///fclose(fisier);
    Deseneaza();
}

int coliziune(int x1, int y1, int marime1, int x2, int y2, int marime2)
{
    if(y2+marime2 >= y1-marime1 && y2+marime2 <= y1+marime1)
    {
        if(x1-marime1 <= x2-marime2 && x1+marime1 >= x2-marime2)
            return 1;
        if(x1-marime1 <= x2+marime2 && x1+marime1 >= x2+marime2)
            return 1;
    }
    if(y2-marime2 >= y1-marime1 && y2-marime2 <= y1+marime1)
    {
        if(x1-marime1 <= x2-marime2 && x1+marime1 >= x2-marime2)
            return 1;
        if(x1-marime1 <= x2+marime2 && x1+marime1 >= x2+marime2)
            return 1;
    }
    if((x1-marime1 <= x2 && x1+marime1 >= x2) && (y1-marime1 <= y2 && y1+marime1 >= y2))
        return 1;
    if((x2-marime2 <= x1 && x2+marime2 >= x1) && (y2-marime2 <= y1 && y2+marime2 >= y1))
        return 1;
    return 0;
}

int estePiesa(int x, int y)
{
    int i, xPiesa, yPiesa, marimePiesa;
    for(i=1; i<=Circuit.NumarNoduri; i++)
        if(Circuit.Noduri[i].sters==0)
        {
            xPiesa=Circuit.Noduri[i].x;
            yPiesa=Circuit.Noduri[i].y;
            marimePiesa=Circuit.Noduri[i].marime;
            if(x < xPiesa+marimePiesa && x > xPiesa-marimePiesa && y < yPiesa+marimePiesa && y > yPiesa-marimePiesa)
                return i;
        }
    return 0;
}

int esteLinie(int x,int y)
{
    int i, xLinie, yLinie;
    for(i=1; i<=nrLinii; i++)
        if(linii[i].sters==0)
        {
            xLinie=linii[i].mijlocx;
            yLinie=linii[i].mijlocy;
            if(Distanta_dintre_2_puncte(xLinie-x,yLinie-y)<=hitbox)
            {
                return i;
            }
        }
    return 0;
}

void punePiesa(int x, int y)
{
    char fisierPuncte[101];
    strcpy(fisierPuncte, "puncteLegatura\\");
    strcat(fisierPuncte, piese[piesaSelectata]);
    strcat(fisierPuncte, ".txt");

    Circuit.NumarNoduri++;
    Circuit.Noduri[Circuit.NumarNoduri].TipNod=piesaSelectata;
    Circuit.Noduri[Circuit.NumarNoduri].x=x;
    Circuit.Noduri[Circuit.NumarNoduri].y=y;
    Circuit.Noduri[Circuit.NumarNoduri].marime=dimensiunePiesa;
    Circuit.Noduri[Circuit.NumarNoduri].orientare=orientare;
    Circuit.Noduri[Circuit.NumarNoduri].valoare=valoare;

    actualizare(Circuit.NumarNoduri,x,y);
    Deseneaza();
}

void leagaPiesa(int index, int x, int y)
{
    int legaturaPunct1, legaturaLibera=0, gasit=0, xPunct, yPunct, Punct2, legaturaPunct2;
    char key='p';

    legaturaPunct1=Find(Circuit.Noduri[index].P, x, y);
    for(int i=1; i<=Circuit.NumarNoduri; i++)
        if(i!=index)
            for(int j=1; j<=Circuit.Noduri[i].P.numarLegaturi; j++)
                if(Circuit.Noduri[i].P.Leg[j].viz==0 && Circuit.Noduri[i].sters==0)
                    legaturaLibera=1;

    if(legaturaLibera==1 && legaturaPunct1!=-1)
    {
        seLeaga=1;
        Deseneaza();
        while(gasit==0)
        {
            if(kbhit())
            {
                key=getch();
                if(key=='x')
                    gasit=1;
            }
            if(ismouseclick(WM_LBUTTONDOWN) && gasit==0)
            {
                xPunct=mousex();
                yPunct=mousey();
                clearmouseclick(WM_LBUTTONDOWN);
                Punct2=estePiesa(xPunct, yPunct);
                if(Punct2 && Punct2 != index && Circuit.Noduri[Punct2].sters==0)
                {
                    legaturaPunct2=Find(Circuit.Noduri[Punct2].P, xPunct, yPunct);
                    if(legaturaPunct2!=-1)
                        gasit=1;
                }
            }
        }
        if(key!='x')
        {
            Circuit.Noduri[index].P.Leg[legaturaPunct1].viz=1;
            Circuit.Noduri[Punct2].P.Leg[legaturaPunct2].viz=1;

            nrLinii++;
            linii[nrLinii].x1=Circuit.Noduri[index].P.Leg[legaturaPunct1].x;
            linii[nrLinii].y1=Circuit.Noduri[index].P.Leg[legaturaPunct1].y;
            linii[nrLinii].x2=Circuit.Noduri[Punct2].P.Leg[legaturaPunct2].x;
            linii[nrLinii].y2=Circuit.Noduri[Punct2].P.Leg[legaturaPunct2].y;
            linii[nrLinii].piesa1=index;
            linii[nrLinii].piesa2=Punct2;
            linii[nrLinii].legatura1=legaturaPunct1;
            linii[nrLinii].legatura2=legaturaPunct2;
        }
        seLeaga=0;
        Deseneaza();
    }
}

void desenareSaveLoad(int proiect, int optiune)
{
    page=1-page;
    setactivepage(page);
    clearviewport();

    int i, j, casutax=rezolutiex/5, casutay=rezolutiey/5;
    char circuit[101];

    for(i=0; i<=6; i=i+2)
        for(j=0; j<=6; j=j+2)
        {
            if(i/2*3+j/2+1 <= numarProiecte)
            {
                strcpy(circuit, "proiecte");
                strcat(circuit, "\\");
                strcat(circuit, "circuit0");
                circuit[strlen(circuit)-1]=(i/2*3+j/2+1)+'0';
                strcat(circuit, ".jpg");
                readimagefile(circuit, i*casutax+grosimeLinie+1, j*casutay+grosimeLinie+1, (i+1)*casutax-grosimeLinie-1, (j+1)*casutay-grosimeLinie-1);
                patrat(i*casutax+grosimeLinie+1, j*casutay+grosimeLinie+1, (i+1)*casutax-grosimeLinie-1, (j+1)*casutay-grosimeLinie-1);
            }
            if((i/2*3+j/2+1)==numarProiecte+1 && proiect==1 && (i/2*3+j/2+1) <= numarMaximProiecte)
            {
                patrat(i*casutax+grosimeLinie+1, j*casutay+grosimeLinie+1, (i+1)*casutax-grosimeLinie-1, (j+1)*casutay-grosimeLinie-1);
                line(i*casutax+casutax/2, j*casutay+casutay/4, i*casutax+casutax/2, (j+1)*casutay-casutay/4);
                line(i*casutax+casutax/4, j*casutay+casutay/2, (i+1)*casutax-casutax/4, j*casutay+casutay/2);
            }
            if(optiune==(i/2*3+j/2+1) && ((optiune <= numarProiecte+1 && proiect==1) || (optiune <= numarProiecte && proiect==2)))
                patrat(i*casutax, j*casutay, (i+1)*casutax, (j+1)*casutay);
        }

    settextstyle(SANS_SERIF_FONT, 0, marimeTextMare);

    patrat(4*casutax, 8*casutay/2, 5*casutax, 9*casutay/2);
    bgiout<<"Confirma";
    outstreamxy(9*casutax/2-textwidth("Confirma")/2, 17*(casutay/4)-textheight("Confirma")/2);

    patrat(4*casutax, 9*casutay/2, 5*casutax, 10*casutay/2-5);
    bgiout<<"Renunta";
    outstreamxy(9*casutax/2-textwidth("Renunta")/2, 19*(casutay/4)-textheight("Renunta")/2);

    settextstyle(SANS_SERIF_FONT, 0, marimeTextMica);
    magie();
    setvisualpage(page);
}

void salvare(int proiect)
{
    int i, j;
    char s[101], SalvarePiese[101], SalvareLinii[101], circuit[101], SalvareRezolutie[101];

    if(proiect==numarProiecte+1 && proiect <= numarMaximProiecte)
    {
        numarProiecte++;
        FILE *nrproiecte=fopen("numarproiecte.txt", "w");
        fprintf(nrproiecte, "%d", numarProiecte);
        fclose(nrproiecte);
    }

    strcpy(s,"proiecte");
    strcat(s,"\\");

    strcpy(SalvarePiese, s);
    strcpy(circuit, s);
    strcpy(SalvareRezolutie, s);
    strcpy(SalvareLinii, s);

    strcat(SalvarePiese, "piese0");
    SalvarePiese[strlen(SalvarePiese)-1]=proiect+'0';
    strcat(SalvarePiese, ".txt");

    strcat(SalvareRezolutie, "rezolutie0");
    SalvareRezolutie[strlen(SalvareRezolutie)-1]=proiect+'0';
    strcat(SalvareRezolutie, ".txt");

    strcat(SalvareLinii, "linii0");
    SalvareLinii[strlen(SalvareLinii)-1]=proiect+'0';
    strcat(SalvareLinii, ".txt");

    strcat(circuit, "circuit0");
    circuit[strlen(circuit)-1]=proiect+'0';
    strcat(circuit, ".jpg");

    writeimagefile(circuit, 0, 4*rezolutiey/32+(grosimeLinie+1)/2, 26*rezolutiex/32-(grosimeLinie+1)/2, rezolutiey);

    FILE *piesele=fopen(SalvarePiese, "w");
    FILE *liniile=fopen(SalvareLinii, "w");
    FILE *rezolutie=fopen(SalvareRezolutie, "w");

    fprintf(rezolutie, "%d %d", rezolutiex, rezolutiey);
    fprintf(piesele,"%d\n", Circuit.NumarNoduri);

    for(i=1; i<=Circuit.NumarNoduri; i++)
    {
        fprintf(piesele,"%d %d %d %d ", Circuit.Noduri[i].TipNod, Circuit.Noduri[i].x, Circuit.Noduri[i].y, Circuit.Noduri[i].marime);
        fprintf(piesele,"%d %d %d %d ", Circuit.Noduri[i].orientare, Circuit.Noduri[i].sters, Circuit.Noduri[i].valoare, Circuit.Noduri[i].P.numarLegaturi);

        for(j=1; j<=Circuit.Noduri[i].P.numarLegaturi; j++)
            fprintf(piesele,"%d %d %d\n", Circuit.Noduri[i].P.Leg[j].x, Circuit.Noduri[i].P.Leg[j].y, Circuit.Noduri[i].P.Leg[j].viz);
    }

    fprintf(liniile,"%d\n",nrLinii);
    for(i=1; i<=nrLinii; i++)
    {
        fprintf(liniile,"%d %d %d %d %d %d ",linii[i].x1, linii[i].y1, linii[i].x2, linii[i].y2, linii[i].piesa1, linii[i].piesa2);
        fprintf(liniile,"%d %d %d\n",linii[i].legatura1, linii[i].legatura2, linii[i].sters);
    }

    fclose(piesele);
    fclose(liniile);
    fclose(rezolutie);
}

void incarcare(int proiect)
{
    int i, j, rezX, rezY, dimPiesa;
    char s[101], SalvarePiese[101], SalvareLinii[101], circuit[101], SalvareRezolutie[101];

    strcpy(s, "proiecte");
    strcat(s, "\\");

    strcpy(SalvarePiese, s);
    strcpy(circuit, s);
    strcpy(SalvareRezolutie, s);
    strcpy(SalvareLinii, s);

    strcat(SalvarePiese, "piese0");
    SalvarePiese[strlen(SalvarePiese)-1]=proiect+'0';
    strcat(SalvarePiese, ".txt");

    strcat(SalvareRezolutie, "rezolutie0");
    SalvareRezolutie[strlen(SalvareRezolutie)-1]=proiect+'0';
    strcat(SalvareRezolutie, ".txt");

    strcat(SalvareLinii, "linii0");
    SalvareLinii[strlen(SalvareLinii)-1]=proiect+'0';
    strcat(SalvareLinii, ".txt");

    strcat(circuit, "circuit0");
    circuit[strlen(circuit)-1]=proiect+'0';
    strcat(circuit, ".jpg");

    FILE *piesele=fopen(SalvarePiese, "r");
    FILE *liniile=fopen(SalvareLinii, "r");
    FILE *rezolutie=fopen(SalvareRezolutie, "r");

    fscanf(rezolutie, "%d%d", &rezX, &rezY);
    dimPiesa=min(rezX, rezY)/11;

    fscanf(piesele, "%d", &Circuit.NumarNoduri);
    for(i=1; i<=Circuit.NumarNoduri; i++)
    {
        fscanf(piesele, "%d%d%d%d", &Circuit.Noduri[i].TipNod, &Circuit.Noduri[i].x, &Circuit.Noduri[i].y, &Circuit.Noduri[i].marime);
        Circuit.Noduri[i].x=Calcul(Circuit.Noduri[i].x, rezX, rezolutiex);
        Circuit.Noduri[i].y=Calcul(Circuit.Noduri[i].y, rezY, rezolutiey);
        Circuit.Noduri[i].marime=Calcul(Circuit.Noduri[i].marime, dimPiesa, dimensiunePiesaStandard);
        fscanf(piesele, "%d%d%d%d", &Circuit.Noduri[i].orientare, &Circuit.Noduri[i].sters, &Circuit.Noduri[i].valoare,&Circuit.Noduri[i].P.numarLegaturi);

        for(j=1; j<=Circuit.Noduri[i].P.numarLegaturi; j++)
        {
            fscanf(piesele, "%d%d%d", &Circuit.Noduri[i].P.Leg[j].x, &Circuit.Noduri[i].P.Leg[j].y, &Circuit.Noduri[i].P.Leg[j].viz);
            Circuit.Noduri[i].P.Leg[j].x=Calcul(Circuit.Noduri[i].P.Leg[j].x, rezX, rezolutiex);
            Circuit.Noduri[i].P.Leg[j].y=Calcul(Circuit.Noduri[i].P.Leg[j].y, rezY, rezolutiey);
        }
    }

    fscanf(liniile, "%d", &nrLinii);
    for(i=1; i<=nrLinii; i++)
    {
        fscanf(liniile, "%d%d%d%d", &linii[i].x1, &linii[i].y1, &linii[i].x2, &linii[i].y2);
        linii[i].x1=Calcul(linii[i].x1, rezX, rezolutiex);
        linii[i].y1=Calcul(linii[i].y1, rezY, rezolutiey);
        linii[i].x2=Calcul(linii[i].x2, rezX, rezolutiex);
        linii[i].y2=Calcul(linii[i].y2, rezY, rezolutiey);

        fscanf(liniile, "%d%d%d%d%d", &linii[i].piesa1, &linii[i].piesa2, &linii[i].legatura1, &linii[i].legatura2, &linii[i].sters);
    }

    fclose(piesele);
    fclose(liniile);
    fclose(rezolutie);
}

void meniuSaveLoad(int proiect)
{
    inSaveLoad=1;
    int x, y, casutax=rezolutiex/5, casutay=rezolutiey/5, optiune=-1;

    while(inSaveLoad==1)
    {
        desenareSaveLoad(proiect, optiune);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            x=mousex();
            y=mousey();
            clearmouseclick(WM_LBUTTONDOWN);
            if(x >= 4*casutax && x <= 5*casutax && y >= 8*casutay/2 && y <= 9*casutay/2)
            {
                if(optiune!=-1 && optiune <= numarProiecte+1)
                {
                    if(proiect==1 && optiune <= numarProiecte+1)
                    {
                        Deseneaza();
                        salvare(optiune);
                    }
                    if(proiect==2 && optiune <= numarProiecte)
                    {
                        Deseneaza();
                        incarcare(optiune);
                    }
                }
                inSaveLoad=0;
            }
            else
                if(x >= 4*casutax && x <= 5*casutax && y >= 9*casutay/2 && y <= 10*casutay/2)
                    inSaveLoad=0;
                else
                {
                    x=x/casutax;
                    y=y/casutay;
                    if(x%2==0 && y%2==0)
                    {
                        x=x/2;
                        y=y/2;
                        y++;
                        optiune=x*3+y;
                    }
                }
        }
    }
}

int nrcifre(int x)
{
    if(x==0) return 1;
    int aux=0;
    while(x>0)
    {
        aux++;
        x/=10;
    }
    return aux;
}

void desenareMeniu()
{
    int i;
    char numar[101];
    strcpy(numar,"1");
    for(i=2;i<=nrcifre(valoare);i++)
    {
        strcat(numar,"1");
    }

    setvisualpage(page2);
    setactivepage(1-page2);
    clearviewport();
    page2=1-page2;

    settextstyle(SANS_SERIF_FONT, 0, marimeTextMare);
    bgiout<<"Valoare:";
    outstreamxy(3*rezolutiex/10-textwidth("Valoare:")/2,1*rezolutiey/20-textheight("V")/2);

    bgiout<<valoare;
    outstreamxy(5*rezolutiex/10-textwidth(numar)/2,1*rezolutiey/20-textheight("1")/2);
    settextstyle(SANS_SERIF_FONT, 0, (marimeTextMica+marimeTextMare)/2);
    patrat(4*rezolutiex/10, 1*rezolutiey/20-textheight("1"), 6*rezolutiex/10, 1*rezolutiey/20+textheight("1"));


    patrat(3*rezolutiex/10, 1*rezolutiey/10, 7*rezolutiex/10, 5*rezolutiey/10);
    readimagefile("background.jpg", 3*rezolutiex/10, 1*rezolutiey/10, 7*rezolutiex/10, 5*rezolutiey/10);

    desenarePiesa(piesaSelectata, rezolutiex/2, 3*rezolutiey/10, dimensiunePiesa, orientare);
    readimagefile("Left.jpg", 1*rezolutiex/10, 1*rezolutiey/10, 3*rezolutiex/10-5, 5*rezolutiey/10);
    readimagefile("Right.jpg", 7*rezolutiex/10+5, 1*rezolutiey/10, 9*rezolutiex/10, 5*rezolutiey/10);

    settextstyle(SANS_SERIF_FONT, 0, marimeTextMare);

    bgiout<<"Piese:";
    outstreamxy(2*rezolutiex/20-textwidth("Piese:")/2, 6*rezolutiey/10-textheight("P")/2);
    settextstyle(SANS_SERIF_FONT, 0, marimeTextMica);

    for(int i=1; i<=nrPiese; i++)
    {
        desenarePiesa(i, (1+i*2)*rezolutiex/20+dimensiunePiesaStandard/2, 11*rezolutiey/20+dimensiunePiesaStandard/2, dimensiunePiesaStandard/2, 1);
        patrat((1+i*2)*rezolutiex/20, 11*rezolutiey/20, (2+i*2)*rezolutiex/20, 13*rezolutiey/20);
        patrat((1+i*2)*rezolutiex/20-7, 11*rezolutiey/20-7, (2+i*2)*rezolutiex/20+7, 13*rezolutiey/20+7);
        bgiout<<piese[i];
        outstreamxy((1+i*2)*rezolutiex/20-textwidth(piese[i])/2+dimensiunePiesaStandard/2, 11*rezolutiey/20+dimensiunePiesaStandard+textheight(piese[i]));
    }

    settextstyle(SANS_SERIF_FONT, 0, marimeTextMare);

    bgiout<<"Marime:";
    outstreamxy(2*rezolutiex/20-textwidth("Marime:")/2, 31*rezolutiey/40-textheight("M")/2);

    bgiout<<dimensiunemin;
    outstreamxy(4*rezolutiex/20-textwidth("50")/2, 15*rezolutiey/20-textheight("5"));

    bgiout<<dimensiunemax;
    outstreamxy(16*rezolutiex/20-textwidth("500")/2, 15*rezolutiey/20-textheight("5"));

    patrat(4*rezolutiex/20, 15*rezolutiey/20, 16*rezolutiex/20, 16*rezolutiey/20);

    int x=dimensiunePiesa-dimensiunemin;
    x=x*(12*rezolutiex/20);
    x=x/(dimensiunemax-dimensiunemin);
    x=x+4*rezolutiex/20;

    setfillstyle(SOLID_FILL, WHITE);
    bar(x-dimensiunePiesaStandard/5, 29*rezolutiey/40, x+dimensiunePiesaStandard/5, 33*rezolutiey/40);
    patrat(4*rezolutiex/20, 17*rezolutiey/20, 16*rezolutiex/20, 19*rezolutiey/20);

    bgiout<<"Iesire";
    outstreamxy(10*rezolutiex/20-textwidth("Iesire")/2, 18*rezolutiey/20-textheight("I")/2);
    settextstyle(SANS_SERIF_FONT, 0, marimeTextMica);

    setvisualpage(page2);
    magie();
}

void meniu()
{
    char key;
    setactivepage(page);
    setvisualpage(page);
    clearviewport();

    desenareMeniu();

    int inMeniu=1, x, y, i;
    while(inMeniu==1)
        {
            if(kbhit())
            {
                key=getch();
                if(key>='0'&&key<='9'&&nrcifre(valoare)<=6)
                {
                    valoare=valoare*10+key-'0';
                    desenareMeniu();
                }
                if(key==8)
                {
                    valoare=valoare/10;
                    desenareMeniu();
                }
            }
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                x=mousex();
                y=mousey();
                if(x >= 1*rezolutiex/10 && x <= 3*rezolutiex/10-5 && y >= 1*rezolutiey/10 && y <= 5*rezolutiey/10)
                    orientare--;

                if(x >= 7*rezolutiex/10+5 && x <= 9*rezolutiex/10 && y >= 1*rezolutiey/10 && y <= 5*rezolutiey/10)
                    orientare++;

                if(y >= 11*rezolutiey/20 && y <= 13*rezolutiey/20)
                    for(i=1; i<=nrPiese; i++)
                        if(x >= (1+2*i)*rezolutiex/20 && x <= (2+2*i)*rezolutiex/20)
                        {
                            if(seSchimba == 1)
                            {
                                if(estePunct != 1 && i!=4)
                                    piesaSelectata=i;
                            }
                            else
                                piesaSelectata=i;
                            orientare=1;
                        }

                patrat(4*rezolutiex/20, 17*rezolutiey/20, 16*rezolutiex/20, 19*rezolutiey/20);

                if(x >= 4*rezolutiex/20 && x <= 16*rezolutiex/20 && y >= 17*rezolutiey/20 && y <= 19*rezolutiey/20)
                    inMeniu=0;

                if(orientare>4)
                    orientare=orientare-4;

                if(orientare<1)
                    orientare=orientare+4;

                if(x >= 4*rezolutiex/20 && y >= 15*rezolutiey/20 && x <= 16*rezolutiex/20 && y <= 16*rezolutiey/20)
                {
                    clearmouseclick(WM_LBUTTONUP);
                    while(ismouseclick(WM_LBUTTONDOWN))
                    {
                        x=mousex();
                        if(x >= 4*rezolutiex/20 && x <= 16*rezolutiex/20)
                        {
                            x=x-4*rezolutiex/20+1;
                            x=(dimensiunemax-dimensiunemin)*x/(12*rezolutiex/20);
                            dimensiunePiesa=x+dimensiunemin;
                        }
                        if(ismouseclick(WM_LBUTTONUP))
                        {
                            x=mousex();
                            if(x >= 4*rezolutiex/20 && x <= 16*rezolutiex/20)
                            {
                                x=x-4*rezolutiex/20+1;
                                x=(dimensiunemax-dimensiunemin)*x/(12*rezolutiex/20);
                                dimensiunePiesa=x+dimensiunemin;
                            }
                            clearmouseclick(WM_LBUTTONDOWN);
                            clearmouseclick(WM_LBUTTONUP);
                        }
                        desenareMeniu();
                    }
                }
                clearmouseclick(WM_LBUTTONDOWN);
                desenareMeniu();
            }
        }
    magie();
}

void desenareMeniuInfo(int x, int y)
{
    page=1;
    setactivepage(page);
    setvisualpage(page);
    clearviewport();

    line(26*rezolutiex/32, 0, 26*rezolutiex/32, rezolutiey);
    line(0, 4*rezolutiey/32, 26*rezolutiex/32, 4*rezolutiey/32);

    settextstyle(SANS_SERIF_FONT, 0, marimeTextMare);
    bgiout<<"ELECTRON";
    outstreamxy(13*rezolutiex/32-textwidth("ELECTRON")/2, 2*rezolutiey/32-textheight("ELECTRON")/2);
    settextstyle(SANS_SERIF_FONT, 0, (marimeTextMare+marimeTextMica)/2);

    for(int i=x; i<=y; i++)
        bgiout<<informatii[i];

    outstreamxy(rezolutiex/8, 2*rezolutiey/8);
    bgiout<<"Orice tasta\npentru a\ncontinua";
    outstreamxy(29*rezolutiex/32-textwidth("Orice tasta")/2, 6*rezolutiey/8);
    settextstyle(SANS_SERIF_FONT, 0, marimeTextMica);

    getch();
}

void meniuInfo()
{
    int numarPagini;
    if(sfarsit%randuriPePagina==0)
        numarPagini=sfarsit/randuriPePagina;
    else
        numarPagini=sfarsit/randuriPePagina+1;

    for(int i=1; i<=numarPagini; i++)
        desenareMeniuInfo((i-1)*randuriPePagina+1, i*randuriPePagina);
}

void mutare_piesa()
{
    Deseneaza();
    if(Circuit.NumarNoduri>0)
    {
        int piesa_de_mutat, x, y, piesa_pusa=0, i, ok=0;
        while(ok==0 && mutat==1)
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                x=mousex();
                y=mousey();
                clearmouseclick(WM_LBUTTONDOWN);

                if(x >= buton[4].x1 && x <= buton[4].x2 && y >= buton[4].y1 && y <= buton[4].y2)
                    mutat=0;

                piesa_de_mutat=estePiesa(x, y);
                if(piesa_de_mutat!=0)
                    ok=1;
            }

        while(WM_LBUTTONDOWN && piesa_pusa==0 && mutat==1)
        {
            x=mousex();
            y=mousey();

            if(ismouseclick(WM_LBUTTONDOWN))
            {
                piesa_pusa=1;
                for(i=1; i<=Circuit.NumarNoduri; i++)
                    if(i!=piesa_de_mutat && Circuit.Noduri[i].sters==0)
                        if((coliziune(x, y, Circuit.Noduri[piesa_de_mutat].marime, Circuit.Noduri[i].x, Circuit.Noduri[i].y, Circuit.Noduri[i].marime)))
                            piesa_pusa=0;
                if(piesa_pusa==1)
                    mutat=0;
                clearmouseclick(WM_LBUTTONDOWN);
            }
            actualizare(piesa_de_mutat,x,y);
        }
    }
}

void sterge_piesa(int piesa_selectata)
{
    Circuit.Noduri[piesa_selectata].sters=1;
    for(int i=1; i<=nrLinii; i++)
        if(linii[i].piesa1==piesa_selectata || linii[i].piesa2==piesa_selectata)
        {
            linii[i].sters=1;
            Circuit.Noduri[linii[i].piesa1].P.Leg[linii[i].legatura1].viz=0;
            Circuit.Noduri[linii[i].piesa2].P.Leg[linii[i].legatura2].viz=0;
        }
}

void sterge_legatura(int piesa1)
{
    int i, x, y, piesa2, gasit=0;
    char key;

    while(gasit==0)
    {
        if(kbhit())
        {
            key=getch();
            if(key=='x')
            {
                gasit=1;
                sterge=0;
            }
        }

        if(ismouseclick(WM_RBUTTONDOWN))
        {
            x=mousex();
            y=mousey();
            clearmouseclick(WM_RBUTTONDOWN);

            piesa2=estePiesa(x, y);
            if(piesa2 && gasit==0)
                for(i=1; i<=nrLinii && gasit==0; i++)
                {
                    if(((linii[i].piesa1==piesa1 && linii[i].piesa2==piesa2) || (linii[i].piesa1==piesa2 && linii[i].piesa2==piesa1)) && (linii[i].sters==0))
                    {
                        gasit=1;
                        amStersLeg=1;
                        linii[i].sters=1;
                        Circuit.Noduri[linii[i].piesa1].P.Leg[linii[i].legatura1].viz=0;
                        Circuit.Noduri[linii[i].piesa2].P.Leg[linii[i].legatura2].viz=0;
                    }
                }
        }
    }
    seLeaga=0;
    Deseneaza();
}

void stergere()
{
    if(Circuit.NumarNoduri>0)
    {
        int gasit=0, x, y, piesa_selectata;
        while(gasit==0 && sterge)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                x=mousex();
                y=mousey();
                clearmouseclick(WM_LBUTTONDOWN);

                if(x >= buton[5].x1 && x <= buton[5].x2 && y >= buton[5].y1 && y <= buton[5].y2)
                    sterge=0;

                piesa_selectata=estePiesa(x, y);
                if(piesa_selectata!=0)
                {
                    gasit=1;
                    sterge_piesa(piesa_selectata);
                }
            }

            if(ismouseclick(WM_RBUTTONDOWN))
            {
                x=mousex();
                y=mousey();
                clearmouseclick(WM_RBUTTONDOWN);

                piesa_selectata=estePiesa(x, y);
                if(piesa_selectata!=0 && avem_legaturi())
                {
                    seLeaga=1;
                    Deseneaza();
                    amStersLeg=0;
                    sterge_legatura(piesa_selectata);
                    gasit=amStersLeg;
                }
            }
        }
    }
}

void setari(int x, int y)
{
    int selectat=verificaButon(x, y);
    if(selectat!=-1)
    {
        switch(selectat)
        {
            case 1:
                meniu();
                clearall();
                break;
            case 2:
                meniuSaveLoad(1);
                clearall();
                Deseneaza();
                break;
            case 3:
                meniuSaveLoad(2);
                clearall();
                Deseneaza();
                break;
            case 4:
                mutat=1;
                mutare_piesa();
                clearall();
                mutat=0;
                break;
            case 5:
                sterge=1;
                Deseneaza();
                stergere();
                clearall();
                sterge=0;
                Deseneaza();
                break;
            case 6:
                meniuInfo();
                clearall();
                Deseneaza();
                break;
            case 7:
                sejoaca=0;
                break;
        }
    }
    Deseneaza();
}

void verificare(int x, int y)
{
    int ok=0, i;

    for(i=1; i<=Circuit.NumarNoduri && ok==0; i++)
        if(coliziune(Circuit.Noduri[i].x, Circuit.Noduri[i].y, Circuit.Noduri[i].marime, max(x,dimensiunePiesa/2), min(y,rezolutiey-dimensiunePiesa/2), dimensiunePiesa)==1 && Circuit.Noduri[i].sters==0)
            ok=1;
    if(ok==0)
        punePiesa(max(x,dimensiunePiesa/2), min(y,rezolutiey-dimensiunePiesa/2));
    else
        if(estePiesa(x, y))
            leagaPiesa(estePiesa(x, y), x, y);
}

void schimba(int x,int y)
{
    int piesaDeSchimbat,linieDeSchimbat;
    int auxPiesaSelectata,auxOrientare,auxDimensiunePiesa,auxValoare;
    piesaDeSchimbat=estePiesa(x,y);
    if(piesaDeSchimbat!=0)
    {
        seSchimba=1;
        auxPiesaSelectata=piesaSelectata;
        auxOrientare=orientare;
        auxDimensiunePiesa=dimensiunePiesa;
        auxValoare=valoare;

        if(Circuit.Noduri[piesaDeSchimbat].TipNod==4) estePunct=1;
        valoare=Circuit.Noduri[piesaDeSchimbat].valoare;
        piesaSelectata=Circuit.Noduri[piesaDeSchimbat].TipNod;
        orientare=Circuit.Noduri[piesaDeSchimbat].orientare;
        dimensiunePiesa=Circuit.Noduri[piesaDeSchimbat].marime;

        meniu();
        if(estePunct==0&&piesaSelectata!=4)
            Circuit.Noduri[piesaDeSchimbat].TipNod=piesaSelectata;
        Circuit.Noduri[piesaDeSchimbat].marime=dimensiunePiesa;
        Circuit.Noduri[piesaDeSchimbat].orientare=orientare;
        actualizare(piesaDeSchimbat,Circuit.Noduri[piesaDeSchimbat].x,Circuit.Noduri[piesaDeSchimbat].y);

        seSchimba=0;
        estePunct=0;
        valoare=auxValoare;
        piesaSelectata=auxPiesaSelectata;
        orientare=auxOrientare;
        dimensiunePiesa=auxDimensiunePiesa;
        Deseneaza();
    }
    else
    {
        linieDeSchimbat=esteLinie(x,y);
        if(linieDeSchimbat!=0)
        {
            swap(linii[linieDeSchimbat].x1,linii[linieDeSchimbat].x2);
            swap(linii[linieDeSchimbat].y1,linii[linieDeSchimbat].y2);
            swap(linii[linieDeSchimbat].piesa1,linii[linieDeSchimbat].piesa2);
            swap(linii[linieDeSchimbat].legatura1,linii[linieDeSchimbat].legatura2);
        }
        Deseneaza();
    }
}

int main()
{
    FILE *info=fopen("Info.txt", "r");
    while(!feof(info))
    {
        sfarsit++;
        fgets(informatii[sfarsit],1001,info);
    }
    fclose(info);

    FILE *nrproiecte=fopen("numarproiecte.txt", "r");
    fscanf(nrproiecte, "%d", &numarProiecte);
    fclose(nrproiecte);

    dimensiunePiesaStandard=min(rezolutiex, rezolutiey)/11;

    marimeTextMare=dimensiunePiesaStandard/14;
    marimeTextMica=marimeTextMare/2;

    dimensiunemax=5*dimensiunePiesaStandard/6;
    dimensiunemin=dimensiunePiesaStandard/2;

    grosimeLinie=(marimeTextMare+marimeTextMica)/2;

    hitbox=grosimeLinie*3;

    citirePiese();

    initwindow(rezolutiex, rezolutiey, "Electron.exe", -3, -25);
    butoane();
    setlinestyle(0, 0, grosimeLinie);
    Deseneaza();

    int x, y;
    while(sejoaca)
    {
        if(kbhit())
            getch();

        if(ismouseclick(WM_LBUTTONDOWN))
        {
            x=mousex();
            y=mousey();
            clearmouseclick(WM_LBUTTONDOWN);
            if(x <= 26*rezolutiex/32-dimensiunePiesa/2 && y >= 4*rezolutiey/32+dimensiunePiesa/2)
            {
                verificare(x,y);
                clearall();
            }
            if(x >= 26*rezolutiex/32)
                setari(x, y);
        }
        if(ismouseclick(WM_RBUTTONDOWN))
        {
            x=mousex();
            y=mousey();
            clearmouseclick(WM_RBUTTONDOWN);
            if(x <= 26*rezolutiex/32-dimensiunePiesa/2 && y >= 4*rezolutiey/32+dimensiunePiesa/2)
            {
                schimba(x, y);
                clearall();
            }
        }
    }
    return 0;
}
