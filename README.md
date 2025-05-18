Install libportaudio
Install wxWidgets 3.0.5



g++ -shared -fPIC DelayPlugin.cpp -o delayPlugin.so 
cd audioInterface/
g++ -c audioInterface.cpp -o audioInterface.cpp.o
cd ../pluginInterface/
g++ -c PluginInterface.cpp -o PluginInterface.cpp.o 
cd ..
g++ -c ApplicationProcessor.cpp -o ApplicationProcessor.cpp.o
ar rcs libapplicationprocessor.a ./audioInterface/audioInterface.cpp.o ./pluginInterface/PluginInterface.cpp.o ApplicationProcessor.cpp.o 
g++ -o app App.cpp -L . -lapplicationprocessor `pkg-config portaudiocpp --libs --cflags` `wx-config --cxxflags --libs`
