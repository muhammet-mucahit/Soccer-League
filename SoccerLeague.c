/* 
 * MERVE AYDIN 330208
 * FATMA MUTLU 341381
 * 02.05.2019
 * Türkiye Sanal Süper Lig takımlarının lig fikstürünün hesaplamasını yapan program.
 */

// KUTUPHANELER
#include <stdio.h>

// TAKMA AD SAYISINI GOSTERIR => A, B, C ..
#define TAKMA_AD_SAYISI 24

// TAKMA ADLAR BU CHAR DIZISI UZERINDEN ATANIR
char TAKMA_ADLAR[TAKMA_AD_SAYISI] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                     'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                     'R', 'S', 'T', 'V', 'W', 'X', 'Y', 'Z'};

// AYARLAR
int TAKIM_SAYISI;
int PUAN_GALIBIYET;
int PUAN_BERABERLIK;
int PUAN_MAGLUBIYET;

// TAKIM BILGILERINI TUTAR
struct Takim
{
    char takma_isim;
    char takim_adi[12];
    int oynadigi_mac_sayisi;
    int attigi_gol_sayisi;
    int yedigi_gol_sayisi;
    int galibiyet_sayisi;
    int beraberlik_sayisi;
    int maglubiyet_sayisi;
    int puan;
    int averaj;
};

// Takimlarin baslangic degerleri 0'a setlenir.
void takimSifirla(struct Takim *takim)
{
    (*takim).attigi_gol_sayisi = 0;
    (*takim).averaj = 0;
    (*takim).beraberlik_sayisi = 0;
    (*takim).galibiyet_sayisi = 0;
    (*takim).maglubiyet_sayisi = 0;
    (*takim).oynadigi_mac_sayisi = 0;
    (*takim).puan = 0;
    (*takim).yedigi_gol_sayisi = 0;
}

// Takim sayisi, galibiyet, maglubiyet, beraberlik puanlari okunur.
void ayarlariOku()
{
    FILE *filePointer;
    filePointer = fopen("ayarlar.txt", "r"); //r okumak için metin dosyası açar

    if (filePointer == NULL)
    {
        printf("Dosya bulunamadı.");
    }

    while (!feof(filePointer))
    { //feof dosya sonu geldiğinde doğru bir değer verir

        fscanf(filePointer, "%d\n%d\n%d\n%d", &TAKIM_SAYISI, &PUAN_GALIBIYET, &PUAN_BERABERLIK, &PUAN_MAGLUBIYET);
    }
    fclose(filePointer);
}

// Takma adi verilen takimin hangisi oldugunu bulur. (Kacinci takim oldugunu bulur indis olarak)
int takmaAdIndisBul(char ch)
{
    int i;
    for (i = 0; i < TAKMA_AD_SAYISI; i++)
    {
        if (ch == TAKMA_ADLAR[i])
            return i;
    }
    return -1;
}

// Takimlar dosyadan okunur ve takma adlari atanir.
void takimlariOku(struct Takim takimlar[])
{
    FILE *filePointer;
    filePointer = fopen("takimlar.txt", "r"); //r okumak için metin dosyası açar

    if (filePointer == NULL)
    {
        printf("Dosya bulunamadı.");
    }

    int i = 0;
    while (!feof(filePointer))
    { //feof dosya sonu geldiğinde doğru bir değer verir
        fscanf(filePointer, "%s\n", takimlar[i].takim_adi);
        takimlar[i].takma_isim = TAKMA_ADLAR[i];
        takimSifirla(&takimlar[i]);
        i++;
    }
    fclose(filePointer);
}

// Takimlarin puan durumunda gozukmesi icin averaj ve puan hesabi gibi islemler yapilir.
void takimlarinSonBilgileriniHesapla(struct Takim takimlar[])
{
    int i;
    for (i = 0; i < TAKIM_SAYISI; i++)
    {
        takimlar[i].averaj = takimlar[i].attigi_gol_sayisi - takimlar[i].yedigi_gol_sayisi;
        takimlar[i].puan = PUAN_GALIBIYET * takimlar[i].galibiyet_sayisi +
                           PUAN_BERABERLIK * takimlar[i].beraberlik_sayisi +
                           PUAN_MAGLUBIYET * takimlar[i].maglubiyet_sayisi;
    }
}

// Her bir takimin galibiyet, maglubiyet, attigi gol vb.. bilgilerini gunceller.
void macBilgileriniTakimlaraAta(struct Takim takimlar[], char ev_sahibi_takma, int ev_sahibi_gol, char konuk_takma, int konuk_gol)
{
    int ev_sahibi_indis = takmaAdIndisBul(ev_sahibi_takma);
    int konuk_indis = takmaAdIndisBul(konuk_takma);

    takimlar[ev_sahibi_indis].attigi_gol_sayisi += ev_sahibi_gol;
    takimlar[konuk_indis].attigi_gol_sayisi += konuk_gol;

    takimlar[ev_sahibi_indis].yedigi_gol_sayisi += konuk_gol;
    takimlar[konuk_indis].yedigi_gol_sayisi += ev_sahibi_gol;

    takimlar[ev_sahibi_indis].oynadigi_mac_sayisi++;
    takimlar[konuk_indis].oynadigi_mac_sayisi++;

    if (ev_sahibi_gol > konuk_gol)
    {
        takimlar[ev_sahibi_indis].galibiyet_sayisi++;
        takimlar[konuk_indis].maglubiyet_sayisi++;
    }
    else if (ev_sahibi_gol == konuk_gol)
    {
        takimlar[ev_sahibi_indis].beraberlik_sayisi++;
        takimlar[konuk_indis].beraberlik_sayisi++;
    }
    else
    {
        takimlar[konuk_indis].galibiyet_sayisi++;
        takimlar[ev_sahibi_indis].maglubiyet_sayisi++;
    }
}

// MACLARI DOSYADAN OKUR
void maclariDosyadanOku(struct Takim takimlar[])
{
    FILE *filePointer;
    filePointer = fopen("maclar.txt", "r"); //r okumak için metin dosyası açar

    if (filePointer == NULL)
    {
        printf("Dosya bulunamadı.");
    }

    char ev_sahibi_takma, konuk_takma;
    int ev_sahibi_gol, konuk_gol;
    while (!feof(filePointer))
    { //feof dosya sonu geldiğinde doğru bir değer verir
        fscanf(filePointer, "%c %d %c %d\n", &ev_sahibi_takma, &ev_sahibi_gol, &konuk_takma, &konuk_gol);
        macBilgileriniTakimlaraAta(takimlar, ev_sahibi_takma, ev_sahibi_gol, konuk_takma, konuk_gol);
    }
    fclose(filePointer);
}

// Klavyeden bir adet mac okur.
void klayveyedenMacAl(struct Takim takimlar[])
{
    char ev_sahibi_takma, konuk_takma;
    int ev_sahibi_gol, konuk_gol;
    printf("Lütfen maç bilgisini giriniz..\n");
    scanf("%c %d %c %d", &ev_sahibi_takma, &ev_sahibi_gol, &konuk_takma, &konuk_gol);
    // getchar();
    macBilgileriniTakimlaraAta(takimlar, ev_sahibi_takma, ev_sahibi_gol, konuk_takma, konuk_gol);
}

// Siralamalarin sonucunda olusan takimlarin puan durumunu ekrana basar.
void yazdir(struct Takim takimlar[])
{
    int i;
    printf("%-5s %-25s %-5s %-5s %-5s %-5s %-5s %-5s %-5s %-5s\n", "Takma", "Takım", "O", "G", "B", "M", "A", "Y", "Av", "P");
    for (i = 0; i < TAKIM_SAYISI; i++)
    {
        printf("%-5c %-25s %-5d %-5d %-5d %-5d %-5d %-5d %-5d %-5d\n",
               takimlar[i].takma_isim,
               takimlar[i].takim_adi,
               takimlar[i].oynadigi_mac_sayisi,
               takimlar[i].galibiyet_sayisi,
               takimlar[i].beraberlik_sayisi,
               takimlar[i].maglubiyet_sayisi,
               takimlar[i].attigi_gol_sayisi,
               takimlar[i].yedigi_gol_sayisi,
               takimlar[i].averaj,
               takimlar[i].puan);
    }
}

// Varsayilan olarak takimlar takma ada gore sirali oldugu icin direkt burda yazdirilir.
void takmaAdaGoreSiralama(struct Takim takimlar[])
{
    yazdir(takimlar);
}

// Takimlari puanlarina, averajlarina veya attigi gol sayilarina gore siralar.
void puanaGoreSirala(struct Takim takimlar[])
{
    struct Takim tempTakimlar[TAKIM_SAYISI];

    int k;
    for (k = 0; k < TAKIM_SAYISI; k++)
    {
        tempTakimlar[k] = takimlar[k];
    }

    struct Takim temp;
    int i, j;
    for (i = 0; i < TAKIM_SAYISI; i++)
    {
        for (j = 0; j < TAKIM_SAYISI - 1; j++)
        {
            if (tempTakimlar[j].puan < tempTakimlar[j + 1].puan)
            {
                temp = tempTakimlar[j];
                tempTakimlar[j] = tempTakimlar[j + 1];
                tempTakimlar[j + 1] = temp;
            }
            else if (tempTakimlar[j].puan == tempTakimlar[j + 1].puan)
            {
                if (tempTakimlar[j].averaj < tempTakimlar[j + 1].averaj)
                {
                    temp = tempTakimlar[j];
                    tempTakimlar[j] = tempTakimlar[j + 1];
                    tempTakimlar[j + 1] = temp;
                }
                else if (tempTakimlar[j].averaj == tempTakimlar[j + 1].averaj)
                {
                    if (tempTakimlar[j].attigi_gol_sayisi < tempTakimlar[j + 1].attigi_gol_sayisi)
                    {
                        temp = tempTakimlar[j];
                        tempTakimlar[j] = tempTakimlar[j + 1];
                        tempTakimlar[j + 1] = temp;
                    }
                }
            }
        }
    }

    yazdir(tempTakimlar);
}

int stringKarsilastir(char *a, char *b)
{
    while (*a != '\0' || *b != '\0')
    {
        if (*a < *b)
            return 1;
        else if (*a > *b)
            return 0;
        a++;
        b++;
    }

    return -1;
}

// Takim isimlerine gore siralama yapar.
void takimIsimlerineGoreSirala(struct Takim takimlar[])
{
    struct Takim tempTakimlar[TAKIM_SAYISI];

    int k;
    for (k = 0; k < TAKIM_SAYISI; k++)
    {
        tempTakimlar[k] = takimlar[k];
    }

    int i, j;
    struct Takim temp;
    for (i = 0; i < TAKIM_SAYISI; i++)
    {
        for (j = 0; j < TAKIM_SAYISI - 1; j++)
        {
            if (stringKarsilastir(tempTakimlar[i].takim_adi, tempTakimlar[i + 1].takim_adi))
            {
                temp = tempTakimlar[j];
                tempTakimlar[j] = tempTakimlar[j + 1];
                tempTakimlar[j + 1] = temp;
            }
        }
    }

    yazdir(tempTakimlar);
}

// Takim isimlerini buyutur.
// Turkce karakter iceren takim isimlerinde hatali cikti uretir.
void takimIsimleriniBuyut(struct Takim takimlar[])
{
    int i;
    char *s;
    for (i = 0; i < TAKIM_SAYISI; i++)
    {
        s = takimlar[i].takim_adi;
        for (; *s != '\0'; s++)
        {
            (*s) -= 32;
        }
    }
}

// KULLANICIYA CESITLI ISLEMLER YAPABILECEGI BIR MENU SUNAR
// Ctrl + C ile cikis yapar.
void menu(struct Takim takimlar[])
{
    int secim;

    while (1)
    {
        printf("[0] - Takımların isimlerini büyüt\n");
        printf("[1] - Mac skoru gir\n");
        printf("[2] - Maçları dosyadan oku\n");
        printf("[3] - Puana gore siralama\n");
        printf("[4] - Takim isimlerine gore siralama\n");
        printf("[5] - Takma isimlere gore siralama\n");

        printf("Secim yapiniz: ");
        scanf("%d", &secim);

        switch (secim)
        {
        case 0:
            takimIsimleriniBuyut(takimlar);
            break;
        case 1:
            klayveyedenMacAl(takimlar);
            break;
        case 2:
            maclariDosyadanOku(takimlar);
            break;
        case 3:
            takimlarinSonBilgileriniHesapla(takimlar);
            puanaGoreSirala(takimlar);
            break;
        case 4:
            takimlarinSonBilgileriniHesapla(takimlar);
            takimIsimlerineGoreSirala(takimlar);
            break;
        case 5:
            takimlarinSonBilgileriniHesapla(takimlar);
            takmaAdaGoreSiralama(takimlar);
            break;
        }
    }
}

int main(void)
{
    ayarlariOku();

    struct Takim takimlar[TAKIM_SAYISI];

    takimlariOku(takimlar);
    menu(takimlar);

    return 0;
}