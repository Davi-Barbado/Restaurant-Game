#include <cstring>
#include <raylib.h>
#include <iostream>

using namespace std;

float window_w = 1280;
float window_h = 720;
// array de ingredientes cru, para não serem adicionados ao prato
int crudes[] = {1, 2, 3};
int crudes_lengh = 3;
const char* dic_ingredients_s[] = {
    "Arroz cru", "Feijão cru", "Bife cru",
    "Arroz", "Feijão", "Bife"
};
int dic_ingredients_lengh = 6;
const char* dic_dishs_s[] = {
    "Arroz e feijão", "Arroz e feijão com bife"
};
class Player{
    private:
        float vel_x, vel_y;
        void input(){
            if (IsKeyDown(KEY_D)){
                vel_x = 5;
                vel_y = 0;
            }
            else if (IsKeyDown(KEY_A)){
                vel_x = -5;
                vel_y = 0;
            }
            else if (IsKeyDown(KEY_W)){
                vel_y = -5;
                vel_x = 0;
            }
            else if (IsKeyDown(KEY_S)){
                vel_y = 5;
                vel_x = 0;
            }
            else{
                vel_x = 0;
                vel_y = 0;
            }
            x += vel_x;
            y += vel_y;
            hitbox.x = x;
            hitbox.y = y;
        }
    public:
        int carrying_ingredient = 0;
        int carrying_dish = 0;
        Rectangle hitbox;
        float x, y;
        Player(float x_, float y_){
            x = x_;
            y = y_;
            hitbox.x = x;
            hitbox.y = y;
            hitbox.width = 10;
            hitbox.height = 10;
        }
        void draw(){
            DrawRectangleRec(hitbox, BLACK);
            // imprimi o nome do prato carregado
            DrawText(carrying_dish != 0 ? dic_dishs_s[carrying_dish - 1]: "", hitbox.x - MeasureText(carrying_dish != 0 ? dic_dishs_s[carrying_dish - 1]: "", 10), hitbox.y, 10, BLACK);
            // imprimi o nome dos ingredientes carregados
            DrawText(carrying_ingredient != 0 ? dic_ingredients_s[carrying_ingredient - 1]: "", hitbox.x - MeasureText(carrying_ingredient != 0 ? dic_ingredients_s[carrying_ingredient - 1]: "", 10), hitbox.y, 10, BLACK);
        }
        void process(){
            input();
        }
        void collision(){
            if (vel_x == 5 && vel_y == 0){
                x -= 5;
            }
            else if (vel_x == -5 && vel_y == 0){
                x += 5;
            }
            else if (vel_x == 0 && vel_y == -5){
                y += 5;
            }
            else if (vel_x == 0 && vel_y == 5){
                y -= 5;
            }
        }

};
class Order{
    public:
        int n1, n2, n3, n4, n5;
        void build(int n1_, int n2_, int n3_, int n4_, int n5_){
            n1 = n1_;
            n2 = n2_;
            n3 = n3_;
            n4 = n4_;
            n5 = n5_; 
        }
        void reset(){
            n1 = 0;
            n2 = 0; 
            n3 = 0;
            n4 = 0;
            n5 = 0;
        }
};
Player p1(window_w/2, window_h/2);
Order orders[10];
int orders_lengh = 0;
class Mount_block{
    private:
        Rectangle hitbox;
        int ingredients[10];
        int ingredients_lengh = 0;
    public:
        int dish = 0;
        void build(int x_, int y_, int width_, int height_){
            hitbox.x = x_;
            hitbox.y = y_;
            hitbox.width = width_;
            hitbox.height = height_;
        }
        void draw(){
            DrawRectangleRec(hitbox, BLUE);
            if (dish == 0){
                for (int x = 0; x < ingredients_lengh; x++){
                    // imprimi o nome de cada ingrediente na bancada
                    DrawText(ingredients[x] != 0 ? dic_ingredients_s[ingredients[x] - 1]: "", hitbox.x - MeasureText(ingredients[x] != 0 ? dic_ingredients_s[ingredients[x] - 1]: "", 10) * x * 4, hitbox.y - 30, 30, BLACK);

                }
            }
            // imprimi o nome dos pratos prontos na bancada
            DrawText(dish != 0 ? dic_dishs_s[dish - 1]: "", hitbox.x - MeasureText(dish != 0 ? dic_dishs_s[dish - 1]: "", 10), hitbox.y - 30, 30, BLACK);
        }
        void process(){
            int points = 0;
            bool is_crude = false;
            for (int x = 0; x < crudes_lengh; x++){
                if (crudes[x] == p1.carrying_ingredient){
                    is_crude = true;
                } 
            }
            if (CheckCollisionRecs(hitbox, p1.hitbox) && p1.carrying_ingredient != 0 && ingredients_lengh < 10 && !(is_crude) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    ingredients[ingredients_lengh] = p1.carrying_ingredient;
                    ingredients_lengh++;
                    p1.carrying_ingredient = 0;
            }
            else if (CheckCollisionRecs(hitbox, p1.hitbox)){
                    p1.collision();
            }
            if (CheckCollisionRecs(hitbox, p1.hitbox) && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && dish != 0){
                    p1.carrying_dish = dish;
                    dish = 0;
                    reset();
            }
            for (int x = 0; x < ingredients_lengh; x++){
                // soma os pontos para verificar qual prato foi montado, exige numeros distintos para evitar combinações inesperadas
                if (ingredients[x] == 4){
                    points += 15;
                }
                else if (ingredients[x] == 5){
                    points += 22;
                }
                else if (ingredients[x] == 6) {
                    points += 35;
                }
                
            }
            if (points == 37){
                dish = 1;    
            }
            else if (points == 72){
                dish = 2;
            }
            if (ingredients_lengh >= 10){
                reset();
            }
        }
        void reset(){
            memset(ingredients, 0, sizeof(ingredients));
            ingredients_lengh = 0;
        }
};
class Delivery_block{
    private:
        Rectangle hitbox;
    public:
        void build(int x_, int y_, int width_, int height_){
            hitbox.x = x_;
            hitbox.y = y_;
            hitbox.width = width_;
            hitbox.height = height_;
        }
        void process(){
            if (CheckCollisionRecs(p1.hitbox, hitbox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                for (int x = 0; x < orders_lengh; x++){
                    // se a player entregar um prato, é verificado se o prato está na lista, se estiver, ele é removido
                    if (p1.carrying_dish == 1){
                        if (orders[x].n1 == 1 && orders[x].n2 == 2 && orders[x].n3 == 0 && orders[x].n4 == 0 && orders[x].n5 == 0){
                            orders[x].reset();
                            p1.carrying_dish = 0;
                        }
                    }
                    else if (p1.carrying_dish == 2){
                        if (orders[x].n1 == 1 && orders[x].n2 == 2 && orders[x].n3 == 3 && orders[x].n4 == 0 && orders[x].n5 == 0){
                            orders[x].reset();
                            p1.carrying_dish = 0;
                        }
                    }
                }
            }
            if (CheckCollisionRecs(hitbox, p1.hitbox)){
                p1.collision();
            }
        }
        void draw(){
            DrawRectangleRec(hitbox, PURPLE);
        }
};
class Ingredient_block{
    public:
        Rectangle hitbox;
        Color c;
        int ingredient;
        const char* label;
        void build(int x, int y, Color c_, int ingredient_, const char* label_){
            hitbox.x = x;
            hitbox.y = y;
            hitbox.width = 50;
            hitbox.height = 50;
            c = c_;
            ingredient = ingredient_;
            label = label_;
        }
        void draw(){
            DrawRectangleRec(hitbox, c);
            DrawText(label, hitbox.x - MeasureText(label, 30), hitbox.y, 30, BLACK);
        }
        void process(){
            if (CheckCollisionRecs(hitbox, p1.hitbox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                p1.carrying_ingredient = ingredient;
            }
            if (CheckCollisionRecs(hitbox, p1.hitbox)){
                p1.collision();
            }
        }
};
class Trash_block{
    private:
        Rectangle hitbox;
        Color c;
    public:
        void build(int x_, int y_, Color c_){
            hitbox.x = x_;
            hitbox.y = y_;
            hitbox.height = 50;
            hitbox.width = 50;
            c = c_;
        }
        void draw(){
            DrawRectangleRec(hitbox, c);
        }
        void process(){
            if (CheckCollisionRecs(hitbox, p1.hitbox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                p1.carrying_dish = 0;
                p1.carrying_ingredient = 0;
            }
            if (CheckCollisionRecs(hitbox, p1.hitbox)){
                p1.collision();
            }
        }
};
class Cook_block{
    private:
        Rectangle hitbox;
        Color c;
        enum State {EMPTY, COOKING, DONE, BURN};
        State state;
        int ingredient = 0;
        float timer = 0;
    public:
        void build(int x, int y, Color c_){
            hitbox.x = x;
            hitbox.y = y;
            hitbox.width = 50;
            hitbox.height = 50;
            c = c_;
            state = EMPTY;
        }
        void draw(){
            DrawRectangleRec(hitbox, BEIGE);
            DrawRectangle(hitbox.x, hitbox.y - 10, 10 * timer, 10, BLUE);
        }
        void process(){
            if (CheckCollisionRecs(hitbox, p1.hitbox)){
                p1.collision();
            }
            switch (state) {
                case EMPTY:
                    if (CheckCollisionRecs(hitbox, p1.hitbox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        bool is_crude = false;
                        for (int x = 0; x < crudes_lengh; x++){
                            if (crudes[x] == p1.carrying_ingredient){
                                is_crude = true;
                            } 
                        }
                        if (is_crude && ingredient == 0){
                            ingredient = p1.carrying_ingredient;
                            p1.carrying_ingredient = 0;
                            state = COOKING;
                        }
                    }
                    break;
                case COOKING:
                    timer += GetFrameTime() / 3;
                    if (timer >= 5.0f){
                        state = DONE;
                        timer = 0;
                    }
                    break;
                case DONE:
                    if (ingredient == 1){
                        ingredient = 4;
                    }
                    else if (ingredient == 2){
                        ingredient = 5;
                    }
                    else if (ingredient == 3){
                        ingredient = 6;
                    }
                    if (CheckCollisionRecs(hitbox, p1.hitbox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && p1.carrying_ingredient == 0){
                        p1.carrying_ingredient = ingredient;
                        ingredient = 0;
                    }
                    else if (CheckCollisionRecs(hitbox, p1.hitbox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && p1.carrying_ingredient != 0){
                        bool is_crude = false;
                        for (int x = 0; x < crudes_lengh; x++){
                            if (crudes[x] == p1.carrying_ingredient){
                                is_crude = true;
                            } 
                        }
                        if (is_crude && ingredient == 0){
                            ingredient = p1.carrying_ingredient;
                            p1.carrying_ingredient = 0;
                            state = COOKING;
                        }
                    }
                    if (ingredient != 0){
                        timer += GetFrameTime() / 6;
                    }
                    else{
                        timer = 0;
                    }
                    if (timer >= 5.0f){
                        state = BURN;
                    }
                    break;
                case BURN:
                    ingredient = 0;
                    timer = 0;
                    if (CheckCollisionRecs(hitbox, p1.hitbox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && p1.carrying_ingredient != 0){
                        bool is_crude = false;
                        for (int x = 0; x < crudes_lengh; x++){
                            if (crudes[x] == p1.carrying_ingredient){
                                is_crude = true;
                            } 
                        }
                        if (is_crude && ingredient == 0){
                            ingredient = p1.carrying_ingredient;
                            p1.carrying_ingredient = 0;
                            state = COOKING;
                        }
                    }
                    break;
            }
        }
};
void create_order(Order o){
    orders[orders_lengh] = o;
    orders_lengh++;
}
const char* GetOrderName(Order o){
    const char* order_name = "";
    if (o.n1 == 1 && o.n2 == 2){
        order_name = "Arroz e Feijão";
    }
    if (o.n1 == 1 && o.n2 == 2 && o.n3 == 3){
        order_name = "Arroz e Feijão e Bife";
    }
    return order_name;
}
Mount_block mount_point;
Delivery_block delivery_point;
Ingredient_block ingredient_block1;
Ingredient_block ingredient_block2;
Ingredient_block ingredient_block3;
Trash_block trash_block;
Cook_block cook_block;
int main(){
    cout << "";
    InitWindow(window_w, window_h, "Game Jam");
    SetTargetFPS(60);
    Order o;
    o.build(1, 2, 0, 0, 0);
    create_order(o);
    Order o2;
    o2.build(1, 2, 3, 0, 0);
    create_order(o2);
    mount_point.build(window_w/2, 300, 50, 50);
    delivery_point.build(window_w/2 + 200, 300, 50, 50);
    ingredient_block1.build(300, 300, RED, 1, "Arroz cru");
    ingredient_block2.build(300, 350, GREEN, 2, "Feijão cru");
    ingredient_block3.build(300, 400, YELLOW, 3, "Bife cru");
    trash_block.build(300, 450, BROWN);
    cook_block.build(window_w/2 - 50, 300, BEIGE);
    while (!(WindowShouldClose())) {
        ClearBackground(RAYWHITE);
        BeginDrawing();
            cout << mount_point.dish << endl;
            p1.draw();
            p1.process();
            DrawText(GetOrderName(orders[0]), 0, 0, 30, BLACK);
            DrawText(GetOrderName(orders[1]), 0, 30, 30, BLACK);   
            mount_point.draw(); 
            mount_point.process();  
            ingredient_block1.draw();     
            ingredient_block1.process();
            ingredient_block2.draw();
            ingredient_block2.process();
            ingredient_block3.draw();
            ingredient_block3.process();
            delivery_point.process();
            delivery_point.draw();
            trash_block.draw();
            trash_block.process();
            cook_block.draw();
            cook_block.process();
        EndDrawing();
    }
    CloseWindow();
}