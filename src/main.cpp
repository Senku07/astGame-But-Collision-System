#include <stdio.h>
#include "include/glad/glad.h"
#include "glad.c"
#include "shader.eg"
#include "GLFW/glfw3.h"
#include "glm/common.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_interpolation.hpp"
typedef unsigned int UI;
UI Program;

struct Data
{
    UI VAO;
    UI VBO;
};

Data BoxData()
{
    float Rec[] =
        {
            0, 0,
            0, 1,
            1, 1,
            1, 1,
            1, 0,
            0, 0};
    Data a = {};
    glGenVertexArrays(1, &a.VAO);
    glBindVertexArray(a.VAO);

    glGenBuffers(1, &a.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, a.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Rec), Rec, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);

    return a;
};

void DrawBox(Data a, int transX, int transY, float colorX, float colorY, float colorZ)
{
    glm::mat4 Model(1);
    Model = glm::translate(Model, glm::vec3(transX, transY, 0));
    Model = glm::scale(Model, glm::vec3(25, 25, 0));
    glUniformMatrix4fv(glGetUniformLocation(Program, "Model"), 1, 0, glm::value_ptr(Model));
    glUniform3f(glGetUniformLocation(Program, "ObjColor"), colorX, colorY, colorZ);
    glBindVertexArray(a.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, a.VBO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(500, 500, "Space Game", 0, 0);
    glfwMakeContextCurrent(window);

    gladLoadGL();

    UI Vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Vs, 1, &vertexShaderSource, 0);
    glCompileShader(Vs);

    UI Fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Fs, 1, &fragmentShaderSource, 0);
    glCompileShader(Fs);

    Program = glCreateProgram();
    glAttachShader(Program, Vs);
    glAttachShader(Program, Fs);
    glLinkProgram(Program);
    glUseProgram(Program);

    glm::mat4 Proj(1);
    Proj = glm::ortho(0.0f, 500.0f, 500.0f, 0.0f);
    glUniformMatrix4fv(glGetUniformLocation(Program, "Proj"), 1, 0, glm::value_ptr(Proj));

    Data a = BoxData();
    bool SpaceShip[9] = {
        0,
        1,
        0,
        1,
        1,
        1,
        1,
        0,
        1,
    };
    bool Enemy[6] = {
        1,
        1,
        1,
        0,
        1,
        0,
    };
    UI moveY = 390;
    UI moveX = 182;
    int r = 0;
    int c = 0;

    UI InitialX = 0;
    UI InitialY = 0;
    UI BulletX[50] = {0};
    UI BulletY[50] = {0};
    int BulletSpeed[50] = {0};

    UI EnemyBUlletX[50] = {0};
    UI EnemyBUlletY[50] = {0};
    UI EnemyBUlletCount = 0;
    UI EnemyBUlletSpeed[50] = {0};
    UI BulletCount = 0;
    UI enemyY = 0;
    UI enemyX = 0;

    glfwSwapInterval(6);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 1);

        for (int y = 0; y < 18; y++)
        {
            for (int x = 0; x < 18; x++)
            {
                DrawBox(a, 15 + y * 25 + y * 1, 15 + x * 25 + x * 1, 1, 0, 0);
            }
        };

        for (int y = 0; y < 3; y++)
        {
            for (int x = 0; x < 3; ++x)
            {
                r = 15 + moveY + 25 * y + y * 1;
                c = 15 + moveX + 25 * x + x * 1;
                if (SpaceShip[y * 3 + x] == true)
                {
                    DrawBox(a, c, r, 0, 0, 1);
                    InitialX = c;
                    InitialY = r;
                }
            }
        }

        // TODO:Enemy start here7
        for (int y = 0; y < 2; y++)
        {
            for (int x = 0; x < 3; x++)
            {
                int c = 15 + enemyY + 25 * y + y * 1;
                int r = 15 + enemyX + 25 * x + x * 1;
                if (Enemy[y * 3 + x] == true)
                {

                    DrawBox(a, r, 26 + c, 0, 0, 0);
                    DrawBox(a, 156 + r, 26 + c, 1, 1, 0);
                    DrawBox(a, 312 + r, 26 + c, 1, 1, 1);
                    // enemyX = r;
                    // enemyY = c;
                }
            }
        }

        if (enemyX > 0)
        {
            enemyX -= 26;
        }
        else
        {
            enemyX = 78;
        }

        if (EnemyBUlletCount < 50)
        {
            int index = EnemyBUlletCount++;
            EnemyBUlletX[index] = enemyX;
            EnemyBUlletY[index] = enemyY;
            EnemyBUlletSpeed[index] += 26;
        }
        else
        {
            EnemyBUlletCount = 0;
            for(int i = 0;i < 50;i++)
            {
                EnemyBUlletSpeed[i] = 0;
            }
        }

        for (int i = 0; i < EnemyBUlletCount; i++)
        {
            DrawBox(a, EnemyBUlletX[i], 26+EnemyBUlletY[i] + EnemyBUlletSpeed[i], 1, 1, 1);
            DrawBox(a, 156+EnemyBUlletX[i],26+ EnemyBUlletY[i] + EnemyBUlletSpeed[i], 1, 1, 1);
            DrawBox(a, 312+EnemyBUlletX[i], 26+EnemyBUlletY[i] + EnemyBUlletSpeed[i], 1, 1, 1);
        }
        for (int i = 0; i < EnemyBUlletCount; ++i)
        {
            EnemyBUlletSpeed[i] += 26;
        }

        // FIXME: key
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && moveX <= 364)
        {
            moveX += 26;
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && moveX >= 26)
        {
            moveX -= 26;
        }
        else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && moveY >= 26)
        {
            moveY -= 26;
        }
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && moveY <= 364)
        {
            moveY += 26;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            if (BulletCount < 50)
            {
                int index = BulletCount++;
                BulletX[index] = InitialX;
                BulletY[index] = InitialY;
                BulletSpeed[index] = -26;
            }
            else if (BulletCount >= 50)
            {
                BulletCount = 0;
            }
        }

        // Render
        for (int i = 0; i < BulletCount; ++i)
        {
            DrawBox(a, BulletX[i] - 26, BulletSpeed[i] + BulletY[i] - 78, 1, 1, 0);
        }

        // Update
        for (int i = 0; i < BulletCount; ++i)
        {
            BulletSpeed[i] += -26;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 07;
}