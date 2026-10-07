{ lib
, stdenv
, src
, cmake
, ninja
, qtbase
, wrapQtAppsHook
, libneurosuite
}:

stdenv.mkDerivation {
  pname = "ndmanager";
  version = "3.0.0";
  inherit src;

  nativeBuildInputs = [ cmake ninja wrapQtAppsHook ];
  buildInputs = [ qtbase libneurosuite ];

  meta = {
    description = "Manager for neurophysiological recording parameters and processing";
    homepage = "https://neurosuite.github.io";
    license = lib.licenses.gpl3Plus;
    mainProgram = "ndmanager";
    platforms = lib.platforms.unix;
  };
}
