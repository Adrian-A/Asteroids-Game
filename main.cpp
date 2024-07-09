// UFO PNG: <a href="" title="ufo icons">Ufo icons created by Freepik - Flaticon</a>
// UFO Sound: https://freesound.org/s/122639/
// Background Music: Music by: bensound.com License code: F64XBWBP1GADQV34
// Laser Sound: https://freesound.org/s/413057/
// Explosion Sound: https://freesound.org/people/bareform/sounds/218721/
// Font: https://www.1001freefonts.com/orbitron.font

// ----- Extra Credit -----
// Added background music
// Added sound when bullet is shot
// Added explosion sound
// Created a score counter

#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <cmath>
#include <SFML/Audio.hpp>

using namespace sf;

// Window height and width
const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f; // turns degree to radians

// Animation class
class Animation{
public:
	float Frame, speed; // frame and speed variables
	Sprite sprite;
    std::vector<IntRect> frames;

	Animation(){}

    // Animation constructor
    Animation (Texture &t, int x, int y, int w, int h, int count, float Speed){
	    Frame = 0; // sets frame to 0
        speed = Speed;

        // for loop to make animation look smoother
		for (int i = 0; i < count; i++) {
            frames.push_back(IntRect(x + i * w, y, w, h));
        }

		sprite.setTexture(t); // sets sprites texture
		sprite.setOrigin(w/2,h/2); // sets sprites origin
        sprite.setTextureRect(frames[0]); // defines the part of the texture that's visible as the sprite
	}

    // function that refreshes and updates frame and speed
	void update(){
    	Frame += speed;
		int n = frames.size();
		if (Frame >= n){
            Frame -= n;
        }
		if (n > 0){
            sprite.setTextureRect( frames[int(Frame)] );
        }
	}

    // checks to see if frame and speed is working
	bool isEnd(){
	  return Frame + speed >= frames.size();
	}

};

// Entity class
class Entity{
public:
    float x, y, dx, dy, R, angle;
    bool life;
    std::string name;
    Animation anim;

    Entity(){
        life = 1; // sets life of object to 1
    }

    // constructor that defines member variables
    void settings(Animation &a, int X, int Y, float Angle = 0, int radius = 1){
        anim = a;
        x = X;
        y = Y;
        angle = Angle;
        R = radius;
    }

    virtual void update(){}; // update function from earlier

    // draws window
    void draw(RenderWindow &app){
        // draws objects on to the screen
        anim.sprite.setPosition(x,y);
        anim.sprite.setRotation(angle + 90);
        app.draw(anim.sprite);

        // creates the size and hit box of the objects
        CircleShape circle(R);
        circle.setFillColor(Color(255,0,0,170));
        circle.setPosition(x, y);
        circle.setOrigin(R,R);
        //app.draw(circle);
    }

    // Entity destructor
    virtual ~Entity(){};
};

// asteroid class
class asteroid: public Entity{
public:
    // spawns asteroids
    asteroid(){
        dx= rand() % 8 - 4;
        dy= rand() % 8 - 4;
        name= "asteroid";
        count++; // adds number of asteroids on screen
    }

    // makes asteroids move
    void update(){
        x += dx;
        y += dy;

        // if statements that determines if the asteroid is off-screen
        if (x > W){
            x = 0;
        }
        if (x < 0){
            x = W;
        }
        if ( y > H){
            y = 0;
        }
        if (y < 0){
            y = H;
        }
    }

    static unsigned int getCount(){
        return count;
    }

    // asteroid destructor
    ~asteroid(){
        --count;
    }

private:
    static unsigned int count; // keeps count on number of asteroids
};

// makes asteroid count 0
unsigned int asteroid::count{0};


// bullet class
class bullet: public Entity{
public:
    // creates a bullet function
    bullet(){
        name = "bullet";
    }

    // makes the bullets move
    void  update(){
        dx = cos(angle * DEGTORAD) * 6;
        dy = sin(angle * DEGTORAD) * 6;
        // angle+=rand()%6-3;
        x += dx;
        y += dy;

        // if statement that determines if the bullet has hit an asteroid
        if (x > W || x < 0 || y > H || y < 0) {
            life = 0; // sets asteroid life to 0
        }
    }

};

// player class
class player: public Entity{
public:
    bool thrust; // thrust variable (player speed)

    // creates name for player class
    player(){
        name = "player";
    }

    // function that sets the players movement and speed
    void update(){
        // if statement that sets the players dx and dy if thrust is true
        if (thrust){
            dx += cos(angle * DEGTORAD) * 0.2;
            dy += sin(angle * DEGTORAD) * 0.2;
        }
        // if thrust is false
        else{
            dx *= 0.99;
            dy *= 0.99;
        }

        int maxSpeed = 15; // sets players max speed
        float speed = sqrt(dx * dx + dy * dy); // sets players speed
        if (speed > maxSpeed){
            dx *= maxSpeed/speed;
            dy *= maxSpeed/speed;
        }

        x += dx;
        y += dy;

        // if statements to determine when the player goes off-screen
        if (x > W){
            x = 0;
        }
        if (x < 0){
            x = W;
        }
        if (y > H){
            y = 0;
        }
        if (y < 0){
            y = H;
        }
    }

};

// ufo class
class ufo: public Entity{
public:
    ufo(){
        dx = 4; // ufo's horizontal speed
        dy = 0;
        name = "ufo";
    }

    void update(){
        x += dx;

        // if ufo goes offscreen it disappears
        if (x > W){
            life = 0;
        }
    }
};

// determines if two objects collide
bool isCollide(Entity *a,Entity *b){
  return (b->x - a->x)*(b->x - a->x)+
         (b->y - a->y)*(b->y - a->y)<
         (a->R + b->R)*(a->R + b->R);
}


int main(){
    int score = 0; // score counter

    // sets a timer
    srand(time(0));

    // creates window
    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60);

    // loads all the textures
    Texture t1,t2,t3,t4,t5,t6,t7,t8;
    if (!t1.loadFromFile("images/spaceship.png")) { return EXIT_FAILURE;}
    if (!t2.loadFromFile("images/background.jpg")) {return EXIT_FAILURE;}
    if (!t3.loadFromFile("images/explosions/type_C.png")) {return EXIT_FAILURE;}
    if (!t4.loadFromFile("images/rock.png")) {return EXIT_FAILURE;}
    if (!t5.loadFromFile("images/fire_blue.png")) {return EXIT_FAILURE;}
    if (!t6.loadFromFile("images/rock_small.png")) {return EXIT_FAILURE;}
    if (!t7.loadFromFile("images/explosions/type_B.png")) {return EXIT_FAILURE;}
    if (!t8.loadFromFile("images/ufo.png")) {return EXIT_FAILURE;}

    // opens audio files
    Music m1, m2, m3, m4;
    if (!m1.openFromFile("sounds/ufo_sound.ogg")) {return EXIT_FAILURE;}
    if (!m2.openFromFile("sounds/scifi.ogg")) {return EXIT_FAILURE;}
    if (!m3.openFromFile("sounds/laser.ogg")) {return EXIT_FAILURE;}
    if (!m4.openFromFile("sounds/boom.ogg")) {return EXIT_FAILURE;}

    // loads font file
    Font f1;
    if (!f1.loadFromFile("fonts/orbitron.ttf")) {return EXIT_FAILURE;}

    // sets score counter text
    Text text1;
    text1.setFont(f1);
    text1.setCharacterSize(24);
    text1.setFillColor(Color::White);

    // plays background music
    m2.play();
    m2.setLoop(true);
    m2.setVolume(5);

    // sets the spaceship and background to be smooth
    t1.setSmooth(true);
    t2.setSmooth(true);

    // sets background image
    Sprite background(t2);

    // sets the animation of each object
    Animation sExplosion(t3, 0,0,256,256, 48, 0.5);
    Animation sRock(t4, 0,0,64,64, 16, 0.2);
    Animation sRock_small(t6, 0,0,64,64, 16, 0.2);
    Animation sBullet(t5, 0,0,32,64, 16, 0.8);
    Animation sPlayer(t1, 40,0,40,40, 1, 0);
    Animation sPlayer_go(t1, 40,40,40,40, 1, 0);
    Animation sExplosion_ship(t7, 0,0,192,192, 64, 0.5);
    Animation sUfo(t8, 0, 0, 40, 40, 1, 0);

    // calls the images entities
    std::list<Entity*> entities;

    // sets random points for asteroids to spawn
    for(int i = 0; i < 15; i++){
      asteroid *a = new asteroid();
      a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
      entities.push_back(a);
    }

    // creates a spawn point for the player
    player *p = new player();
    p->settings(sPlayer,200,200,0,20);
    entities.push_back(p);

    // makes it so there is no ufo
    bool ufoExists = false;
    bool startButton = false;

    /////main loop/////
    while (app.isOpen()){
        Event event;
        while (app.pollEvent(event)){
            if (event.type == Event::Closed) {
                app.close();
            }

            // makes ship shoot bullets
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Space) {
                    m3.play();
                    m3.setVolume(10);
                    bullet *b = new bullet();
                    b->settings(sBullet, p->x, p->y, p->angle, 10);
                    entities.push_back(b);
                }
            }
        }

        // if statement that turns ship to the right
        if (Keyboard::isKeyPressed(Keyboard::Right)){
            p->angle += 3;
        }
        // if statement that turns ship to the left
        if (Keyboard::isKeyPressed(Keyboard::Left)){
            p->angle -= 3;
        }
        // if statement that thrust ship forward
        if (Keyboard::isKeyPressed(Keyboard::Up)){
            p->thrust = true;
        }
        // if no key is pressed nothing happens
        else{
            p->thrust = false;
        }



        for(auto a:entities){
            for(auto b:entities){
                // if statement that determines if a bullet and an asteroid collide
                if (a->name == "asteroid" && b->name == "bullet") {
                    // gets rid of the bullet and the asteroid when they collide
                    if (isCollide(a, b)) {
                        score += 1000; // score for hitting asteroid

                        a->life = false;
                        b->life = false;

                        // creates the explosion
                        m4.play();
                        Entity *e = new Entity();
                        e->settings(sExplosion, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);

                        // creates a new asteroid when one is destroyed
                        for (int i = 0; i < 2; i++) {
                            if (a->R == 15){
                                continue;
                            }
                            Entity *e = new asteroid();
                            e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
                            entities.push_back(e);
                        }
                    }
                }

                // if statement to determine if an asteroid and the ship collide
                if (a->name == "player" && b->name == "asteroid") {
                    // gets rid of the ship when it collides
                    if (isCollide(a, b)) {
                        score -= 500; // score for colliding with asteroid

                        b->life = false;

                        // creates the explosion
                        m4.play();
                        Entity *e = new Entity();
                        e->settings(sExplosion_ship, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);

                        // resets the player to the middle of the screen
                        p->settings(sPlayer, W / 2, H / 2, 0, 20);
                        p->dx = 0;
                        p->dy = 0;
                    }
                }

                // if statement to determine if the ufo is shot by a bullet
                if (a->name == "ufo" && b->name == "bullet"){
                    // gets rid of bullet and ufo
                    if (isCollide(a, b)){
                        score += 2000; // score for hitting ufo

                        a->life = false;
                        b->life = false;

                        // creates the explosion
                        m4.play();
                        Entity *e = new Entity();
                        e->settings(sExplosion, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);
                    }
                }

                // if statement to determine if the player collides with the ufo
                if (a->name == "player" && b->name == "ufo"){
                    // gets rid of ufo
                    if (isCollide(a, b)){
                        score -= 1500; // score for colliding with ufo

                        b->life = false;

                        // creates the explosion
                        m4.play();
                        Entity *e = new Entity();
                        e->settings(sExplosion, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);

                        // resets the ships position to the center
                        p->settings(sPlayer, W/2, H/2, 0, 20);
                        p->dx = 0;
                        p->dy = 0;
                    }
                }
            }
        }

        // accelerates the ship when moving
        if (p->thrust){
            p->anim = sPlayer_go;
        }
        // slows down the player when not moving
        else{
            p->anim = sPlayer;
        }

        // objects die when they explode
        for(auto e:entities) {
            if (e->name == "explosion") {
                if (e->anim.isEnd()) {
                    e->life = 0;
                }
            }
        }

        // randomly creates an ufo
        if (rand() % 500 == 0 && ufoExists == false){
            ufoExists = true;
            ufo *u = new ufo();
            u->settings(sUfo, 50, rand() % 600 + 100, 270, 50);
            entities.push_back(u);
            m1.play();
            m1.setLoop(true);
        }

        /*// randomly creates asteroids
        if (rand() % 150 == 0) {
            asteroid *a = new asteroid();
            a->settings(sRock, 0, rand() % H, rand() % 360, 25);
            entities.push_back(a);
        }*/

        // spawns asteroids when all 15 are destroyed
        if(asteroid::getCount() == 0){
            // for loop that spawns all 15 asteroids
            for (int i = 0; i < 15; i++){
                asteroid *a = new asteroid();
                a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
                entities.push_back(a);
            }
        }

        // updates and refreshes objects
        for(auto i=entities.begin();i!=entities.end();){
            Entity *e = *i;

            e->update();
            e->anim.update();

            if (e->life==false){
                if (e->name == "ufo"){
                    ufoExists = false;
                    m1.stop();
                }
                i = entities.erase(i);
                delete e;
            }
            else{
                i++;
            }
        }



        //////draw//////
        app.draw(background); // draws background

        // draws all objects
        for(auto i:entities) {
            i->draw(app);
        }

        // sets the score text
        text1.setString("Score: " + std::to_string(score));
        // displays score counter
        app.draw(text1);

        app.display(); // displays everything
    }

    return 0;
}
