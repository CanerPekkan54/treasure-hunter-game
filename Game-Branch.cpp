#include <iostream> // C++'da girdi ve çıktı işlemleri için gerekli kütüphane
#include <vector> // Vektör veri yapısını kullanmak için kütüphane
#include <cstdlib> // Rastgele sayı üretimi için gerekli kütüphane
#include <ctime> // Zaman tabanlı rastgelelik için kütüphane

using namespace std; // Standart isim alanını (std) kullanabilmek için

// Temel sinif: GameObject (Oyun Nesnesi)
class GameObject {
protected:
    string name; // Nesnenin ismi
    int health; // Nesnenin sağlık puanı

public:
    GameObject(const string& name, int health)
        : name(name), health(health) { // Yapıcı fonksiyon: nesnenin ismi ve sağlığı ayarlanıyor
    }

    virtual void displayStatus() const { // Nesnenin durumunu ekrana yazdıran sanal fonksiyon
        cout << name << " - Saglik: " << health << endl;
    }

    bool isAlive() const { // Nesnenin yaşayıp yaşamadığını kontrol eder
        return health > 0;
    }

    void takeDamage(int damage) { // Nesne hasar aldığında sağlık puanını azaltır
        health -= damage;
        if (health < 0) health = 0; // Sağlık negatif olmamalı
    }

    string getName() const { // Nesnenin ismini döndüren fonksiyon
        return name;
    }
};

// Oyuncu sinifi
class Player : public GameObject { // GameObject sınıfından türetilmiş
private:
    int attackPower; // Oyuncunun süldürme gücü
    int x, y; // Oyuncunun harita üzerindeki konumu

public:
    Player(const string& name, int health, int attackPower, int startX, int startY)
        : GameObject(name, health), attackPower(attackPower), x(startX), y(startY) { // Yapıcı fonksiyon
    }

    void move(char direction, int mapSize) { // Oyuncuyu belirtilen yönde hareket ettiren fonksiyon
        switch (direction) {
        case 'w':
            if (y > 0) y--; // Yukarı hareket
            break;
        case 's':
            if (y < mapSize - 1) y++; // Aşağı hareket
            break;
        case 'a':
            if (x > 0) x--; // Sola hareket
            break;
        case 'd':
            if (x < mapSize - 1) x++; // Sağa hareket
            break;
        default:
            cout << "Gecersiz hareket!" << endl; // Hatalı girdi durumu
        }
        cout << name << " konumuna tasindi: (" << x << ", " << y << ")" << endl; // Yeni konumu gösterir
    }

    void attack(GameObject& target) { // Oyuncunun hedefe saldırdığı fonksiyon
        int damage = attackPower; // Hasar mıktarını belirler
        target.takeDamage(damage); // Hedefe hasar uygular
        cout << name << " " << target.getName() << "'ye saldirdi ve " << damage << " hasar verdi!" << endl; // Saldırıyı bildirir
    }

    void heal() { // Oyuncuyu iyileştiren fonksiyon
        int healAmount = 20; // İyileşme miktarı
        health += healAmount; // Sağlığı arttır
        cout << name << " iyilesti ve " << healAmount << " saglik kazandi. Mevcut saglik: " << health << endl; // Durumu bildirir
    }

    int getX() const { return x; } // Oyuncunun x koordinatını döndürür
    int getY() const { return y; } // Oyuncunun y koordinatını döndürür
};

// Dusman sinifi
class Enemy : public GameObject { // GameObject sınıfından türetilmiş
private:
    int attackPower; // Dusmanın süldürme gücü

public:
    Enemy(const string& name, int health, int attackPower)
        : GameObject(name, health), attackPower(attackPower) { // Yapıcı fonksiyon
    }

    void attack(Player& player) { // Dusmanın oyuncuya saldırdığı fonksiyon
        int damage = attackPower; // Hasar mıktarını belirler
        player.takeDamage(damage); // Oyuncuya hasar uygular
        cout << name << " saldiridi! " << player.getName() << " " << damage << " hasar aldi!" << endl; // Saldırıyı bildirir
    }
};

// Harita sinifi
class Map {
private:
    vector<vector<char>> grid; // Harita için 2D karakter matrisi
    int size; // Harita boyutu
    pair<int, int> treasurePosition; // Hazine konumu

public:
    Map(int size) : size(size) { // Yapıcı fonksiyon, harita boyutunu ayarlar
        grid = vector<vector<char>>(size, vector<char>(size, '.')); // Harita nokta (‘.’) ile doldurulur
        placeTreasure(); // Hazineyi yerleştirir
    }

    void placeTreasure() { // Hazineyi haritada rastgele bir yere yerleştirir
        treasurePosition.first = rand() % size; // Rastgele x koordinatı
        treasurePosition.second = rand() % size; // Rastgele y koordinatı
        grid[treasurePosition.first][treasurePosition.second] = 'T'; // Hazineyi ‘T’ ile işaretler
    }

    bool isTreasure(int x, int y) { // Oyuncunun koordinatlarında hazine olup olmadığını kontrol eder
        return x == treasurePosition.first && y == treasurePosition.second;
    }

    void display(Player& player) { // Haritayı ekrana yazdırır
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i == player.getY() && j == player.getX()) {
                    cout << 'P' << " "; // Oyuncuyu ‘P’ ile gösterir
                }
                else if (i == treasurePosition.first && j == treasurePosition.second) {
                    cout << 'T' << " "; // Hazineyi ‘T’ ile gösterir
                }
                else {
                    cout << grid[i][j] << " "; // Boş alanı nokta ile gösterir
                }
            }
            cout << endl;
        }
    }
};

// Savas sistemi fonksiyonu
void battle(Player& player, Enemy& enemy) {
    cout << "\nBir savas basladi!" << endl; // Savaş başladı bildirimi
    while (player.isAlive() && enemy.isAlive()) { // Oyuncu ve dusman yaşadığı müdetçe döngü devam eder
        cout << "\nOyuncunun sirasi:" << endl;
        cout << "1. Saldir\n2. Iyiles\nBir eylem secin: "; // Oyuncu için seçenekler
        int choice;
        cin >> choice; // Oyuncunun seçimi

        if (choice == 1) {
            player.attack(enemy); // Oyuncu saldırır
        }
        else if (choice == 2) {
            player.heal(); // Oyuncu iyileşir
        }
        else {
            cout << "Gecersiz secim!" << endl; // Hatalı girdi durumu
            continue;
        }

        if (enemy.isAlive()) { // Dusman yaşıyorsa saldırır
            cout << "\nDusmanin sirasi:" << endl;
            enemy.attack(player);
        }
    }

    if (player.isAlive()) {
        cout << "Dusmani yendiniz!" << endl; // Oyuncu kazandı bildirimi
    }
    else {
        cout << "Dusman sizi yendi..." << endl; // Dusman kazandı bildirimi
    }
}

// Ana oyun dongusu
int main() {
    srand(time(0)); // Rastgelelik için zamanı kullan

    cout << "Hazine Avcisi Oyununa Hosgeldiniz!" << endl; // Oyuna karşılama mesajı
    int mapSize = 5; // Harita boyutunu belirle
    Map map(mapSize); // Haritayı oluştur

    Player player("Oyuncu", 100, 25, 0, 0); // Oyuncu oluştur
    Enemy enemy("Dusman", 50, 15); // Dusman oluştur

    while (player.isAlive()) { // Oyuncu yaşadıkça oyun devam eder
        cout << "\nHarita:" << endl;
        map.display(player); // Haritayı göster

        cout << "\nHareket (w: yukari, s: asagi, a: sola, d: saga): "; // Hareket seçenekleri
        char moveDirection;
        cin >> moveDirection; // Oyuncunun hareket yönü

        player.move(moveDirection, mapSize); // Oyuncuyu hareket ettir

        if (map.isTreasure(player.getX(), player.getY())) { // Oyuncu hazinenin konumundaysa
            cout << "Tebrikler! Hazinenizi buldunuz!" << endl; // Kazandı bildirimi
            break;
        }

        if (rand() % 3 == 0) { // Yuzde 33 olasilikla bir dusman cikar
            cout << "\nBir dusman belirdi!" << endl;
            battle(player, enemy); // Savaş başlat

            if (enemy.isAlive()) { // Dusman yaşıyorsa yeni bir dusman yaratılır
                enemy = Enemy("Dusman", 50, 15);
            }
        }

        if (!player.isAlive()) { // Oyuncu öldüyse oyun biter
            cout << "Oyun Bitti! Kaybettiniz. Tekrar deneyin!" << endl;
            break;
        }
    }

    return 0; // Programın başarıyla sona erdiğini bildirir
}