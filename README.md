# Splendor Duel en C++/Qt

Ce projet est une adaptation numérique du célèbre jeu de société **Splendor Duel**, réalisée en C++ avec le framework Qt 6. Il offre une expérience de jeu complète avec une interface graphique, une intelligence artificielle et la possibilité de sauvegarder les parties.

Ce projet a été développé dans le cadre du cours LO21 à l'Université de Technologie de Compiègne (UTC).

---

### 📸 Aperçu du Jeu

<p align="center">
  <img width="800" alt="Aperçu du jeu Splendor Duel" src="https://github.com/user-attachments/assets/1abd58d6-4e96-40ef-9a32-46d43e04f610">
</p>

---

### ✨ Fonctionnalités

*   **Implémentation Complète des Règles :** Respect de la mécanique de jeu de Splendor Duel (prise de jetons, achat de cartes, pyramide, cartes royales, privilèges, conditions de victoire).
*   **Interface Graphique Intuitive :** Interface développée avec **Qt 6**, permettant une interaction fluide avec le plateau, les cartes et les jetons.
*   **Mode de Jeu contre une IA :** Affrontez une intelligence artificielle grâce à un système de stratégie interchangeable.
*   **Gestion des Données via Base de Données :** Les informations sur les cartes (Joaillerie et Royales) sont chargées depuis une base de données **SQLite**.
*   **Système de Sauvegarde :** Possibilité de sauvegarder l'état actuel d'une partie pour la reprendre plus tard (fonctionnalité en cours de développement/finalisation).
*   **Logique de Jeu Robuste :** La partie est gérée par un contrôleur central qui orchestre les tours des joueurs et valide les actions (obligatoires et optionnelles).

---

### 🛠️ Stack Technique

*   **Langage :** C++17
*   **Framework IHM :** Qt 6
*   **Base de Données :** SQLite 3 pour la gestion des cartes et la sauvegarde.
*   **Système de Build :** CMake
*   **Design Patterns :** Singleton, Builder, Stratégie, ...

---

### 🚀 Installation et Lancement

Pour compiler et lancer le projet, vous aurez besoin de CMake, d'un compilateur C++17 et de Qt 6.

1.  **Cloner le dépôt :**
    ```bash
    git clone https://github.com/RAPHCVR/SplendorDuel.git
    cd SplendorDuel
    ```

2.  **Configurer avec CMake :**
    Créez un dossier `build` et configurez le projet. **N'oubliez pas de spécifier le chemin vers votre installation de Qt.**

    ```bash
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=/chemin/vers/votre/installation/Qt
    ```
    *(Exemple : `C:/Qt/6.6.0/mingw_64` ou `/usr/lib/x86_64-linux-gnu/qt6`)*

3.  **Compiler le projet :**
    ```bash
    cmake --build .
    ```
    *(ou utilisez simplement `make` sur Linux/macOS)*

4.  **Lancer l'application :**
    L'exécutable se trouvera dans le dossier `build`.
    ```bash
    ./projet-lo21-splendor-duel 
    ```
    *(Le nom de l'exécutable peut varier en fonction de votre système d'exploitation)*

---

### 📂 Structure du Projet

*   `source/` : Contient l'ensemble du code source du jeu.
    *   **Logique du jeu :** `Partie.h/cpp`, `joueur.h/cpp`, `Controller.h/cpp`, etc.
    *   **Composants du jeu :** `Cards.h/cpp`, `Jeton.h/cpp`.
    *   **Interface Graphique (Qt) :** `QTJeton.h/cpp`, `QTCards.h/cpp`, `QTGame.h/cpp`, etc.
    *   **Stratégie IA :** `strategy.h/cpp`.
    *   **Sauvegarde :** `Save.h/cpp`.
*   `source/sqlite/` : Bibliothèque SQLite utilisée par le projet.
*   `Data/` (implicite) : La base de données `cards.db` est nécessaire au bon fonctionnement et doit se trouver dans le chemin d'accès relatif spécifié dans le code (`source/Data/cards.db`).
*   `Rendu2/`, `Rendu3/` : Archives des différentes étapes de soumission du projet universitaire.

---

### 👥 Auteurs

Ce projet est le fruit du travail de :

*   **Raphaël Chauvier** - Architecture principale, Moteur de jeu (Controller, Game), Logique des jetons, UI/UX (QTGame, QTJeton).
*   **Celine Ozkaya** - Logique Joueur (Back-end) et implémentation du pattern Stratégie.
*   **Lise Leclef** - Interface Joueur (PlayerQT), co-création de la classe Joueur, intégration IA dans l'IHM et tests.
*   **Remy Théolier** - Logique des Cartes (Back-end) et système de Sauvegarde/Reprise.
*   **Sacha Malaterre** - Logique des Cartes (Back-end) et système de Sauvegarde/Reprise.
