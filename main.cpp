#include <GL/glut.h>    // Βιβλιοθήκη OpenGL Utility Toolkit για παράθυρα και αλληλεπίδραση
#include <stdio.h>      // Βιβλιοθήκη για είσοδο/έξοδο
#include <stdbool.h>    // Υποστήριξη τύπου boolean
#include <math.h>       // Μαθηματικές συναρτήσεις, π.χ. sin, cos, fmod

#define WINDOW_WIDTH 800    // Πλάτος παραθύρου σε pixels
#define WINDOW_HEIGHT 800   // Ύψος παραθύρου σε pixels

int currentShape = 4;       // Τρέχον σχήμα (default 4 = κύβος)
float shapeColor[3] = {0.0f, 1.0f, 1.0f}; // Default χρώμα: κυανό (RGB)
bool rotating = false;      // Flag για περιστροφή (αρχικά όχι)
float rotationAngle = 0.0f; // Τρέχουσα γωνία περιστροφής
float scale = 1.0f;         // Κλίμακα μεγέθυνσης/σμίκρυνσης
float translateX = 0.0f, translateY = 0.0f; // Μετατόπιση σε X και Y

// Συνάρτηση για σχεδίαση κειμένου στην οθόνη σε 2D συντεταγμένες (x,y)
void drawText(const char* text, float x, float y) {
    glMatrixMode(GL_PROJECTION);    // Επιλογή projection matrix
    glPushMatrix();                  // Αποθήκευση τρέχουσας μήτρας προβολής
    glLoadIdentity();                // Μηδενισμός μήτρας
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT); // Ορθογραφική προβολή 2D

    glMatrixMode(GL_MODELVIEW);     // Επιλογή modelview matrix
    glPushMatrix();                 // Αποθήκευση τρέχουσας μήτρας μοντέλου
    glLoadIdentity();               // Μηδενισμός μήτρας

    glDisable(GL_LIGHTING);         // Απενεργοποίηση φωτισμού για καθαρό κείμενο
    glColor3f(1.0f, 1.0f, 1.0f);   // Χρώμα λευκό για το κείμενο
    glRasterPos2f(x, y);            // Θέση αρχής κειμένου
    while (*text) {                 // Για κάθε χαρακτήρα στο string
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++); // Σχεδίαση χαρακτήρα
    }
    glEnable(GL_LIGHTING);          // Ενεργοποίηση ξανά φωτισμού

    glPopMatrix();                  // Επαναφορά μήτρας μοντέλου
    glMatrixMode(GL_PROJECTION);   // Επιλογή μήτρας προβολής
    glPopMatrix();                  // Επαναφορά μήτρας προβολής
    glMatrixMode(GL_MODELVIEW);     // Επιστροφή σε μήτρα μοντέλου
}

// Σχεδίαση ακμών κύβου
void drawCubeEdges() {
    glDisable(GL_LIGHTING);
    glColor3f(0,0,0);
    glutWireCube(1.01);
    glEnable(GL_LIGHTING);
}

// Σχεδίαση ακμών ορθογώνιου παραλληλεπιπέδου
void drawRectPrismEdges() {
    glDisable(GL_LIGHTING);
    glColor3f(0,0,0);
    glPushMatrix();
    glScalef(1.0f, 1.5f, 0.5f);
    glutWireCube(1.01);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// Σχεδίαση ακμών πυραμίδας
void drawPyramidEdges() {
    glDisable(GL_LIGHTING);
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP); // βάση τετράγωνου
        glVertex3f(-1,-1,1);
        glVertex3f(1,-1,1);
        glVertex3f(1,-1,-1);
        glVertex3f(-1,-1,-1);
    glEnd();

    glBegin(GL_LINES); // ακμές από βάση στην κορυφή
        glVertex3f(0,1,0); glVertex3f(-1,-1,1);
        glVertex3f(0,1,0); glVertex3f(1,-1,1);
        glVertex3f(0,1,0); glVertex3f(1,-1,-1);
        glVertex3f(0,1,0); glVertex3f(-1,-1,-1);
    glEnd();
    glEnable(GL_LIGHTING);
}

// Σχεδίαση ακμών σφαίρας
void drawSphereEdges() {
    glDisable(GL_LIGHTING);
    glColor3f(0,0,0);
    glutWireSphere(1.01, 16, 16); // λίγο πιο μεγάλο από τη γεμάτη σφαίρα
    glEnable(GL_LIGHTING);
}


// Σχεδίαση κύβου με χρώμα και ακμές
void drawCube() {
    glColor3fv(shapeColor);
    glutSolidCube(1.0);
    drawCubeEdges();
}

// Σχεδίαση ορθογώνιου παραλληλεπιπέδου
void drawRectPrism() {
    glPushMatrix();
    glScalef(1.0f, 1.5f, 0.5f);
    glColor3fv(shapeColor);
    glutSolidCube(1.0);
    glPopMatrix();
    drawRectPrismEdges();
}

// Σχεδίαση εξαγώνου σε 2D
void drawHexagon() {
    glDisable(GL_LIGHTING);        // Απενεργοποίηση φωτισμού
    glBegin(GL_POLYGON);           // Ένα πολυγωνικό σχήμα
    glColor3fv(shapeColor);        // Χρώμα γεμίσματος
    for (int i = 0; i < 6; ++i) { // 6 κορυφές εξαγώνου
        float angle = i * M_PI / 3; // Γωνία κορυφής (60 μοίρες)
        glVertex2f(cos(angle), sin(angle)); // Συντεταγμένες κορυφής
    }
    glEnd();                      // Τέλος πολυγώνου
    glEnable(GL_LIGHTING);        // Ενεργοποίηση φωτισμού
}

// Σχεδίαση κύκλου σε 2D
void drawCircle() {
    glDisable(GL_LIGHTING);        // Απενεργοποίηση φωτισμού
    glBegin(GL_POLYGON);           // Πολυγωνικό σχήμα (γεμάτο)
    glColor3fv(shapeColor);        // Χρώμα γεμίσματος
    for (int i = 0; i < 360; ++i) { // 360 σημεία για ομαλότητα
        float angle = i * M_PI / 180.0; // Γωνία σε ακτίνια
        glVertex2f(cos(angle), sin(angle)); // Συντεταγμένες κύκλου
    }
    glEnd();                      // Τέλος πολυγώνου
    glEnable(GL_LIGHTING);        // Ενεργοποίηση φωτισμού
}

// Σχεδίαση πυραμίδας με τριγωνικές έδρες
void drawPyramid() {
    glColor3fv(shapeColor);
    glBegin(GL_TRIANGLES);
        glVertex3f(0,1,0); glVertex3f(-1,-1,1); glVertex3f(1,-1,1);
        glVertex3f(0,1,0); glVertex3f(1,-1,1); glVertex3f(1,-1,-1);
        glVertex3f(0,1,0); glVertex3f(1,-1,-1); glVertex3f(-1,-1,-1);
        glVertex3f(0,1,0); glVertex3f(-1,-1,-1); glVertex3f(-1,-1,1);
    glEnd();
    drawPyramidEdges();
}

// Σχεδίαση σφαίρας με φωτισμό
void drawSphere() {
    glColor3fv(shapeColor);
    glutSolidSphere(1.0, 30, 30);
    drawSphereEdges();
}

// Η βασική συνάρτηση απεικόνισης που καλείται κάθε καρέ
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Καθαρισμός buffer χρώματος & βάθους
    glLoadIdentity();             // Μηδενισμός μήτρας μοντέλου

    glTranslatef(translateX, translateY, -6.0f); // Μετάθεση σχήματος στο χώρο (z = -6 για απόσταση)
    glScalef(scale, scale, scale); // Κλίμακα σχήματος

    if (rotating)                 // Αν το flag περιστροφής είναι ενεργό
        rotationAngle += 0.5f;   // Αύξηση γωνίας περιστροφής
    rotationAngle = fmod(rotationAngle, 360.0f); // Περιορισμός γωνίας στο 0-360

    // Αν το σχήμα υποστηρίζει φωτισμό (όχι hexagon & circle)
    if (currentShape >= 1 && currentShape <= 6 && currentShape != 2 && currentShape != 3)
        glEnable(GL_LIGHTING);    // Ενεργοποίηση φωτισμού
    else
        glDisable(GL_LIGHTING);   // Απενεργοποίηση φωτισμού

    // Προσθήκη προκαθορισμένων κλίσεων για 3D σχήματα
    if (currentShape >= 1 && currentShape <= 6 && currentShape != 2 && currentShape != 3) {
        glRotatef(30, 1, 0, 0);   // Περιστροφή 30 μοίρες γύρω από X
        glRotatef(30, 0, 1, 0);   // Περιστροφή 30 μοίρες γύρω από Y
    }

    if (currentShape <= 3)        // Αν το σχήμα είναι 2D (rect prism, hexagon, circle)
        glRotatef(rotationAngle, 0, 0, 1); // Περιστροφή γύρω από z άξονα
    else                          // Για 3D σχήματα
        glRotatef(rotationAngle, 0, 1, 0); // Περιστροφή γύρω από y άξονα

    // Επιλογή σχεδίασης ανάλογα με το σχήμα
    switch (currentShape) {
        case 1: drawRectPrism(); break;
        case 2: drawHexagon(); break;
        case 3: drawCircle(); break;
        case 4: drawCube(); break;
        case 5: drawPyramid(); break;
        case 6: drawSphere(); break;
    }

    // Σχεδίαση κειμένου με όνομα και ΑΜ
    drawText("Onomateponymo: Politis Vasilis - AM: 2563", 10, WINDOW_HEIGHT - 25);
    glutSwapBuffers();            // Ανταλλαγή buffers για ομαλή απεικόνιση
}

// Συνάρτηση για χειρισμό αλλαγής μεγέθους παραθύρου
void reshape(int w, int h) {
    glViewport(0, 0, w, h);       // Ορισμός μεγέθους viewport
    glMatrixMode(GL_PROJECTION);  // Επιλογή μήτρας προβολής
    glLoadIdentity();             // Μηδενισμός μήτρας
    gluPerspective(45, (float)w / h, 1, 100); // Προοπτική προβολή με γωνία 45°, αναλογία διαστάσεων, κοντινό και μακρινό κλιπ
    glMatrixMode(GL_MODELVIEW);   // Επιστροφή σε μήτρα μοντέλου
}

// Timer για επαναλαμβανόμενη ανανέωση οθόνης (~60 fps)
void timer(int value) {
    glutPostRedisplay();          // Σήμα για ανανέωση οθόνης
    glutTimerFunc(16, timer, 0); // Επαναπρογραμματισμός timer για επόμενη κλήση (16 ms)
}

// Χειρισμός απλών πλήκτρων πληκτρολογίου
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: exit(0);          // ESC: έξοδος από πρόγραμμα
        break;
        case ' ': rotating = !rotating; // Space: toggle περιστροφή
        break;
        case '+': scale += 0.1f;   // +: μεγέθυνση
        break;
        case '-': scale = (scale > 0.1f) ? scale - 0.1f : scale; // -: σμίκρυνση, με ελάχιστο όριο
        break;
        case '1':                  // Επιλογή χρώματος κυανό
            shapeColor[0] = 0.0f; shapeColor[1] = 1.0f; shapeColor[2] = 1.0f;
        break;
        case '2':                  // Κόκκινο
            shapeColor[0] = 1.0f; shapeColor[1] = 0.0f; shapeColor[2] = 0.0f;
        break;
        case '3':                  // Πράσινο
            shapeColor[0] = 0.0f; shapeColor[1] = 1.0f; shapeColor[2] = 0.0f;
        break;
        case '4':                  // Πορτοκαλί
            shapeColor[0] = 1.0f; shapeColor[1] = 0.5f; shapeColor[2] = 0.0f;
        break;
        case '5':                  // Ιριδίζον χρώμα, μεταβαλλόμενο με τη γωνία περιστροφής
        {
            float t = fmod(rotationAngle / 100.0f, 1.0f);
            shapeColor[0] = t;
            shapeColor[1] = 0.5f;
            shapeColor[2] = 1.0f - t;
            break;
        }
    }
}

// Χειρισμός ειδικών πλήκτρων (βέλη, F1-F6)
void specialKeys(int key, int x, int y) {
    float moveStep = 0.1f;        // Βήμα μετακίνησης
    switch (key) {
        case GLUT_KEY_LEFT:        // Βέλος αριστερά
            if (translateX > -2.0f)
                translateX -= moveStep;
        break;
        case GLUT_KEY_RIGHT:       // Βέλος δεξιά
            if (translateX < 2.0f)
                translateX += moveStep;
        break;
        case GLUT_KEY_UP:          // Βέλος πάνω
            if (translateY < 2.0f)
                translateY += moveStep;
        break;
        case GLUT_KEY_DOWN:        // Βέλος κάτω
            if (translateY > -2.0f)
                translateY -= moveStep;
        break;
        case GLUT_KEY_F1: currentShape = 1; break; // Ορθογώνιο παραλληλεπίπεδο
        case GLUT_KEY_F2: currentShape = 2; break; // Εξάγωνο
        case GLUT_KEY_F3: currentShape = 3; break; // Κύκλος
        case GLUT_KEY_F4: currentShape = 4; break; // Κύβος
        case GLUT_KEY_F5: currentShape = 5; break; // Πυραμίδα
        case GLUT_KEY_F6: currentShape = 6; break; // Σφαίρα
    }
}

// Αρχικοποίηση φωτισμού
void initLighting() {
    GLfloat light_pos[] = {0.0, 0.0, 10.0, 1.0}; // Θέση φωτός στο χώρο (x,y,z,w)
    GLfloat ambient[] = {0.3, 0.3, 0.3, 1.0};    // Περιβάλλον φως (χαμηλής έντασης)

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);  // Ρύθμιση θέσης φωτός
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);     // Ρύθμιση περιβάλλοντος φωτός
    glEnable(GL_LIGHT0);                            // Ενεργοποίηση φωτός 0
    glEnable(GL_COLOR_MATERIAL);                    // Ενεργοποίηση χρωματισμού υλικού
}

// Συνάρτηση μενού για επιλογές σχήματος και χρώματος
void menu(int id) {
    if (id >= 1 && id <= 6) currentShape = id;    // Αν id 1-6, ορισμός σχήματος
    else if (id >= 11 && id <= 15)                 // Αν id 11-15, αλλαγή χρώματος μέσω keyboard()
        keyboard('0' + id - 10, 0, 0);
    glutPostRedisplay();                           // Ανανέωση οθόνης μετά από αλλαγή
}

// Δημιουργία μενού δεξιού κλικ με υπομενού για χρώματα και σχήματα
void createMenu() {
    int colorMenu = glutCreateMenu(menu);        // Δημιουργία μενού χρωμάτων
    glutAddMenuEntry("Kyano", 11);                // Είσοδος κυανό
    glutAddMenuEntry("Kokkino", 12);              // Κόκκινο
    glutAddMenuEntry("Prasino", 13);              // Πράσινο
    glutAddMenuEntry("Portokali", 14);            // Πορτοκαλί
    glutAddMenuEntry("Iridizon", 15);             // Ιριδίζον

    int shapeMenu = glutCreateMenu(menu);        // Δημιουργία μενού σχημάτων
    glutAddMenuEntry("Orthogwnio", 1);            // Ορθογώνιο παραλληλεπίπεδο
    glutAddMenuEntry("Exagwno", 2);               // Εξάγωνο
    glutAddMenuEntry("Kyklos", 3);                 // Κύκλος
    glutAddMenuEntry("Kyvos", 4);                  // Κύβος
    glutAddMenuEntry("Pyramid", 5);                // Πυραμίδα
    glutAddMenuEntry("Sfaira", 6);                  // Σφαίρα

    glutCreateMenu(menu);                          // Κύριο μενού
    glutAddSubMenu("Xrwma", colorMenu);           // Υπομενού χρωμάτων
    glutAddSubMenu("Sxhma", shapeMenu);            // Υπομενού σχημάτων

    glutAttachMenu(GLUT_RIGHT_BUTTON);             // Σύνδεση με δεξί κλικ
}

// Συνάρτηση αρχικοποίησης OpenGL παραμέτρων
void init() {
    glEnable(GL_DEPTH_TEST);    // Ενεργοποίηση δοκιμής βάθους για σωστή απόδοση 3D
    glEnable(GL_LIGHTING);      // Ενεργοποίηση φωτισμού
    initLighting();             // Αρχικοποίηση φωτισμού
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Γκρι σκούρο φόντο
}

// Κύρια συνάρτηση
int main(int argc, char** argv) {
    glutInit(&argc, argv);                          // Αρχικοποίηση GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Διπλή buffer, RGB, βάθος
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // Μέγεθος παραθύρου
    glutCreateWindow("Shapes with OpenGL");          // Δημιουργία παραθύρου

    init();                                         // Αρχικοποίηση ρυθμίσεων OpenGL

    glutDisplayFunc(display);                        // Συνάρτηση απεικόνισης
    glutReshapeFunc(reshape);                        // Συνάρτηση αλλαγής μεγέθους παραθύρου
    glutKeyboardFunc(keyboard);                      // Συνάρτηση πληκτρολογίου
    glutSpecialFunc(specialKeys);                    // Συνάρτηση ειδικών πλήκτρων
    glutTimerFunc(0, timer, 0);                      // Timer για animation

    createMenu();                                   // Δημιουργία μενού
    glutMainLoop();                                
    return 0;                                       
}
