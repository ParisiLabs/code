declare_module(crashreporter .)
add_lib(crashreporter)

require_cxx11_abi(${targ})

function(require_crashreporter targ)
  message(STATUS "Configuring ${targ} with module_crashreporter")

  require_cxx11_abi(${targ})
  target_link_libraries${targ} module_crashreporter)
endfunction()
