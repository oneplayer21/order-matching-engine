#include <iostream>
#include "../include/OrderBook.hpp"


// --- 3. POINT D'ENTRÉE ---
int main() {
    OrderBook book;

    std::cout << "--- ETAPE 1 : Les vendeurs se placent (Makers) ---" << std::endl;
    // Vendeur 1 : vend 10 actions à 100€
    book.addOrder({1, Side::SELL, 100, 10});

    // Vendeur 2 : vend 20 actions à 105€
    book.addOrder({2, Side::SELL, 105, 20});

    // Vendeur 3 : vend 5 actions à 105€ (se met dans la file derrière le Vendeur 2)
    book.addOrder({3, Side::SELL, 105, 5});

    book.printBook();
    // ATTENDU :
    // Asks: 100 (Qté totale: 10, 1 ordre), 105 (Qté totale: 25, 2 ordres)
    // Bids: Vide


    std::cout << "\n--- ETAPE 2 : Un acheteur agressif arrive ---" << std::endl;
    // Acheteur 1 : Veut acheter 15 actions, et il est prêt à payer jusqu'à 105€ !
    // Que doit-il se passer ?
    // 1. Il achète les 10 actions à 100€ (Match total du vendeur 1). Le niveau 100 disparait.
    // 2. Il lui en manque 5. Il achète 5 actions au vendeur 2 à 105€ (Match partiel).
    // 3. Son ordre est terminé (Qté = 0).
    book.addOrder({4, Side::BUY, 105, 15});

    book.printBook();
    // ATTENDU :
    // Asks: 105 (Le premier ordre est passé de 20 à 15, le 2ème est tjrs de 5. File de 2 ordres)
    // Bids: Vide


    std::cout << "\n--- ETAPE 3 : Un acheteur très gourmand ---" << std::endl;
    // Acheteur 2 : Veut acheter 50 actions à 110€ !
    // Que doit-il se passer ?
    // 1. Il achète tout ce qui reste à 105€ (les 15 du Vendeur 2, puis les 5 du Vendeur 3).
    // 2. Le carnet des Asks est maintenant totalement vide.
    // 3. Il lui manque encore 30 actions à acheter.
    // 4. Il va se placer dans le carnet des Bids pour attendre (Maker).
    book.addOrder({5, Side::BUY, 110, 50});

    book.printBook();
    // ATTENDU :
    // Asks: Vide !
    // Bids: 110 (Qté restante: 30, 1 ordre)
    return 0;
}
