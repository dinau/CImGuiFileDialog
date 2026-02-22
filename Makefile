
all:
	$(MAKE) -C examples/glfw_opengl3_base
	$(MAKE) -C examples/glfw_opengl3_color

.PHONY: clean cleanall

clean:
	$(MAKE) -C examples/glfw_opengl3_base clean
	$(MAKE) -C examples/glfw_opengl3_color clean

cleanall:
	$(MAKE) -C examples/glfw_opengl3_base cleanall
	$(MAKE) -C examples/glfw_opengl3_color cleanall

CIMGUI_ORG_DIR = ../cimgui
gen:
	-rm -fr libs/cimgui
	-mkdir -p libs/cimgui/imgui/backends
	-cp  $(CIMGUI_ORG_DIR)/{cimgui*.*,LICENSE,*.md,Makefile}            libs/cimgui/
	-cp  $(CIMGUI_ORG_DIR)/imgui/*.*                                    libs/cimgui/imgui/
	-cp  $(CIMGUI_ORG_DIR)/imgui/backends/imgui_impl_{glfw.*,opengl3*}  libs/cimgui/imgui/backends/
