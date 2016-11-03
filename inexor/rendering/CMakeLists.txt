declare_module(rendering .)
add_lib(rendering) # create the static lib module_rendering

# Acquire our dependencies for this module
require_sdl(module_rendering              NOLINK)
require_util(module_rendering             NOLINK)

# This function is used to bind this module into another module/application
function(require_rendering targ)
  message(STATUS "Configuring ${targ} with module_rendering")

  if (";${ARGN};" MATCHES ";NOLINK;") # Some argument passing, just copy that.
    set(NOLINK "NOLINK")
  endif()

  target_link_libs(${targ} module_rendering ${NOLINK})
  
  require_sdl(${targ} ${NOLINK})
  require_util(${targ} ${NOLINK})

endfunction()
