all:
	$(MAKE) -C examples/glfw_opengl3_base
	$(MAKE) -C examples/glfw_opengl3_color

clean:
	$(MAKE) -C examples/glfw_opengl3_base clean
	$(MAKE) -C examples/glfw_opengl3_color clean
