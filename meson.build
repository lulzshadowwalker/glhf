project('glhf', 'cpp',
        version : '0.1',
        default_options : ['warning_level=3',
        'cpp_std=c++14'])

gl   = dependency('gl')
glfw = dependency('glfw3')

incdir = include_directories('src/vendor/imgui', 'src/')

imgui_src = files(
  'src/vendor/imgui/imgui.cpp',
  'src/vendor/imgui/imgui_draw.cpp',
  'src/vendor/imgui/imgui_impl_glfw.cpp',
  'src/vendor/imgui/imgui_impl_opengl3.cpp',
  'src/vendor/imgui/imgui_widgets.cpp',
  'src/vendor/imgui/imgui_tables.cpp'
)

app_src = files(
  'src/main.cpp',
  'src/IndexBuffer.cpp',
  'src/Renderer.cpp',
  'src/Shader.cpp',
  'src/VertexBuffer.cpp',
  'src/VertexArray.cpp',
  'src/Texture.cpp'
)

test_src = files(
  'src/tests/TestClearColor.cpp',
  'src/tests/TestTexture.cpp',
  'src/tests/Test.cpp',
)

all_src = app_src + imgui_src + test_src

exe = executable('glhf',
                 sources: all_src,
                 include_directories: incdir,
                 dependencies: [gl, glfw],
                 install: true
)

test('basic', exe)
