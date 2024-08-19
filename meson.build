project('glhf', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++14'])

gl = dependency('gl')
glut = dependency('glut')
glfw = dependency('glfw3')

incdir = include_directories('src/vendor/imgui', 'src/')
src = ['src/main.cpp', 'src/IndexBuffer.cpp', 'src/Renderer.cpp', 'src/Shader.cpp', 'src/VertexBuffer.cpp', 'src/VertexArray.cpp', 'src/Texture.cpp', 'src/vendor/imgui/imgui.cpp', 'src/vendor/imgui/imgui_draw.cpp', 'src/vendor/imgui/imgui_impl_glfw.cpp', 'src/vendor/imgui/imgui_impl_opengl3.cpp', 'src/vendor/imgui/imgui_widgets.cpp', 'src/vendor/imgui/imgui_tables.cpp', 'src/tests/TestClearColor.cpp', 'src/tests/Test.cpp']
exe = executable('glhf', 'src/main.cpp',
                 install : true, dependencies : [gl, glut, glfw], sources : src, include_directories : incdir)

test('basic', exe)
