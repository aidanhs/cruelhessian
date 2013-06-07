#! /bin/sh
set -e

# Author install suggestion
#sudo apt-get install libcegui-mk2-dev libsfml-dev cmake libboost-filesystem-dev libboost-system-dev g++

# Trial and error...possibly more needed
sudo apt-get install cmake
sudo apt-get install g++
sudo apt-get install libsfml-dev
sudo apt-get install libcegui-mk2-dev
sudo apt-get install freeglut3-dev
sudo apt-get install libfreetype6-dev
sudo apt-get install libsdl1.2-dev libsdl-mixer1.2-dev libsdl-image1.2-dev
#sudo apt-get install libboost-filesystem1.53-dev libboost-system1.53-dev

sed -i 's|\(LzmaDec.c\)|\1\r\n\tsrc/Jet.cpp|' CMakeLists.txt
sed -i 's|\(InterfaceElement.cpp\)|\1\r\n    src/interface/JetBar.cpp|' CMakeLists.txt
sed -i 's|\(SOIL.c\)|\1\r\n\tsrc/soil/stb_image_aug.c|' CMakeLists.txt
sed -i 's|\(stb_image_aug.c\)|\1\r\n\tsrc/soil/image_helper.c|' CMakeLists.txt

sed -i 's|\(LINK_LIBRARIES(CEGUIBase)\)|LINK_DIRECTORIES(${BOOST_LIBRARYDIR})\r\nLINK_DIRECTORIES(${CEGUI_LIBRARIES})\r\n\r\n\1|' CMakeLists.txt

sed -i 's|\(InterfaceElement\)|interface/\1|' CMakeLists.txt

for f in ConvertUTF.c  ConvertUTF.h  SimpleIni.h; do
  curl https://simpleini.googlecode.com/svn-history/r65/trunk/$f > src/parser/$f
done

mkdir -p deps
cd deps

wget http://sourceforge.net/projects/boost/files/boost/1.38.0/boost_1_38_0.tar.gz
tar xvf boost_1_38_0.tar.gz
cd boost_1_38_0
mkdir dist
./configure --prefix=$(pwd)/dist --with-libraries=filesystem,system
cd dist/lib
ln -s libboost_filesystem*.so.1.38.0 libboost_filesystem.so
ln -s libboost_system*.so.1.38.0 libboost_system.so
cd ../..
cd ..

wget http://prdownloads.sourceforge.net/crayzedsgui/CEGUI-0.6.2b.tar.gz
tar xvf CEGUI-0.6.2b.tar.gz
cd CEGUI-0.6.2/
mkdir dist
sed -i 's/<stdexcept>/<stdexcept>\n#include <cstddef>/g' include/CEGUIString.h
./configure --prefix=$(pwd)/dist
make
make install
cd ..

mkdir data
cd data
wget http://cruelhessian.konp.eu/cruelhessian-links.txt
DATASERVER="$(grep Server cruelhessian-links.txt | sed 's".*=\s*\(.*[^/]\)/\?$"\1"g')"
while read line; do
  if echo $line | grep -q "^Path"; then
    DATAFILE="$(echo $line | sed 's/.*=\s*//')"
    wget "$DATASERVER/$file"
  fi
  read line
  if echo $line | grep -q "^MD5"; then
    echo "$DATAFILE"
    MD5="$(echo $line | sed 's/.*=\s*//')"
    if [ "$(md5sum -b $DATAFILE)" != "$MD5 *$DATAFILE" ]; then
      echo "WARNING: INVALID MD5 FOR $DATAFILE"
    fi
  fi
done <cruelhessian-links.txt
mkdir -p ~/.config/cruelhessian
for data in *.tar.lzma; do
  cp "$data" ~/.config/cruelhessian/
  (cd ~/.config/cruelhessian/ && lzma --stdout -d "$data" | tar xf - && rm $data)
done
cd ..

cd ..

cmake -Wno-dev --warn-uninitialized --warn-unused-vars -D "Boost_INCLUDE_DIR:PATH=$(pwd)/deps/boost_1_38_0/dist/include/boost-1_38" -D "BOOST_LIBRARYDIR:PATH=$(pwd)/deps/boost_1_38_0/dist/lib" -D "CEGUI_INCLUDE_DIRS:PATH=$(pwd)/deps/CEGUI-0.6.2/dist/include/CEGUI" -D "CEGUI_LIBRARIES:PATH=$(pwd)/deps/CEGUI-0.6.2/dist/lib" .

