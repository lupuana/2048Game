# 2048 Game Implementation  

🌟 This project was developed as an assignment for the first-year class **"Computer Programming and Programming Languages"**. The goal was to recreate the popular 2048 game using the C programming language, while following key specifications. This project uses the `ncurses` library for a terminal-based graphical interface, allowing users to interact with the game through a visually engaging experience.  

---

## Functionality Overview  

### **menu**
Displays the main menu where users can select one of the following options:  
- **New Game**: Starts a fresh game.  
- **Resume**: Continues a saved game from the last session.  
- **Quit**: Exits the program.
 
The menu highlights the current selection and navigates using arrow keys.  

### **writeMatrix**
Saves the current game state (matrix, score, and best score) to a file for later use.  

### **readMatrix**
Reads the saved game state (matrix, score, and best score) from a file to resume a previous session.  

### **generateNew**
Generates a new random tile (2 or 4) in an empty cell of the game matrix after a valid move.  

### **moveUp, moveDown, moveLeft, moveRight**  
Handles the logic for moving tiles in the specified direction:
- Merges tiles with the same value.
- Shifts all non-zero tiles to the correct position.
- Updates the score and the best score if applicable.
- Generates a new tile if the move results in any change to the matrix.

### **how**
Displays a pop-up explaining the rules of the game:  
- Use arrow keys to move tiles.  
- Tiles with the same number merge when they touch.  
- The goal is to reach the 2048 tile.  

### **winMessage**
Displays a congratulatory message when the player achieves the 2048 tile.  

### **loseMessage**
Displays a message when no more valid moves are available, indicating that the game is over.  

### **winner**
Checks if the player has won by finding a 2048 tile in the matrix.  

### **loser**
Checks if the player has lost by determining whether there are no empty cells or valid moves remaining.  

### **show**
Renders the game board with colored tiles based on their values, updating the visuals after every move.  

### **newGame**
Handles the initialization of a new game:
- Clears the matrix and resets the score for a fresh start.
- Places two random tiles on the board.
- Includes the main gameplay loop for user input (arrow keys, `x` for help, `q` to quit).  

### **main**
The entry point of the program:
- Initializes the `ncurses` environment and sets up color pairs for the UI.  
- Displays the menu and manages the game state transitions (new game, resume, quit).  

---

## Challenges  

This assignment was particularly challenging because:  
1. **First-time use of `ncurses`**: Learning to implement a graphical interface in a terminal environment required adapting to a new library and its conventions.  
2. **Structuring a larger program**: As my first bigger programming assignment, organizing code and managing multiple functionalities was difficult. The result is functional but not as modular or clean as it could be.  

Despite these challenges, this project was a valuable learning experience and helped me build confidence in handling more complex programming tasks.  
