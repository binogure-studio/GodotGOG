def can_build(platform):
  return platform=="x11" or platform=="windows" or platform=="osx"

def configure(env):
  if (env['build_gog'] != 'no'):
    env.Append(CPPPATH=["#modules/godotgog/sdk/Include/"])
    if env["platform"]== "x11":
      env.Append(CXXFLAGS="-std=c++0x")
      if env["bits"]=="32":
        env.Append(LIBS=["Galaxy"])
        env.Append(RPATH=[env.Literal('\\$$ORIGIN/linux32')])
        env.Append(LIBPATH=["#modules/godotgog/sdk/Libraries/linux32"])
      else: # 64 bit
        env.Append(LIBS=["Galaxy64"])
        env.Append(RPATH=[env.Literal('\\$$ORIGIN/linux64')])
        env.Append(LIBPATH=["#modules/godotgog/sdk/Libraries/linux64"])
    elif env["platform"] == "windows":
      # mostly VisualStudio
      if env["CC"] == "cl":
        if env["bits"]=="32":
          env.Append(LINKFLAGS=["Galaxy.lib"])
          env.Append(LIBPATH=["#modules/godotgog/sdk/Libraries/win32"])
        else: # 64 bit
          env.Append(LINKFLAGS=["Galaxy64.lib"])
          env.Append(LIBPATH=["#modules/godotgog/sdk/Libraries/win64"])
      # mostly "gcc"
      else:
        if env["bits"]=="32":
          env.Append(LIBS=["Galaxy"])
          env.Append(LIBPATH=["#modules/godotgog/sdk/Libraries/win32"])
        else: # 64 bit
          env.Append(LIBS=["Galaxy64"])
          env.Append(LIBPATH=["#modules/godotgog/sdk/Libraries/win64"])
    elif env["platform"] == "osx":
      env.Append(CXXFLAGS="-std=c++0x")
      env.Append(LIBS=["Galaxy64"])
      env.Append(LIBPATH=['#modules/godotgog/sdk/Libraries/osx'])
