CPP = g++ -std=c++11
INC = -I/usr/local/include -I.

# >>> FOR LINUX, uncomment next few lines; comment out the MAC ones.
C_FLAGS = -fPIC -g -c $(INC) -DGL_GLEXT_PROTOTYPES
GL_LIB_LOC = -L/usr/lib/nvidia-375
GL_LIBRARIES = $(GL_LIB_LOC) -lglfw -lGLU -lGL
MAKE = make
# >>> FOR MAC, uncomment next few lines; comment out previous linux ones.
# C_FLAGS = -fPIC -g -c $(INC) -DGLFW_INCLUDE_GLEXT -DGLFW_INCLUDE_GLCOREARB -DGL_SILENCE_DEPRECATION
# GL_LIBRARIES = -L/usr/local/lib -lglfw -framework OpenGL
# MAKE = make -f MakefileMac
# >>> END: FOR LINUX - FOR MAC

LINK = g++ -fPIC -g

OBJS = hello.o ModelView.o GLFWController.o ShaderIF.o

hello: $(OBJS)
	$(LINK) -o hello $(OBJS) $(GL_LIBRARIES)

hello.o: hello.c++
	$(CPP) $(C_FLAGS) hello.c++
ModelView.o: ModelView.c++
	$(CPP) $(C_FLAGS) ModelView.c++
GLFWController.o: GLFWController.c++
	$(CPP) $(C_FLAGS) GLFWController.c++
ShaderIF.o: ShaderIF.h ShaderIF.c++
	$(CPP) $(C_FLAGS) ShaderIF.c++
