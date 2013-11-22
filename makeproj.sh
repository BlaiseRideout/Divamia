#!/bin/bash

echo -n "Name: "
read name

lowername=$(echo $name | awk '{print tolower($0)}')
uppername=$(echo $name | awk '{print toupper($0)}')

sed "s/divamia/${lowername}/g" makefile-template > makefile

if [ ! -f "src/${lowername}.hpp" ] && [ ! -f "src/${lowername}.cpp" ]; then
	echo -e "#ifndef ${uppername}_H" > "src/${lowername}.hpp"
	echo -e "#define ${uppername}_H\n" >> "src/${lowername}.hpp"
	
	echo -e '#include "game.hpp"\n\n'>> "src/${lowername}.hpp"

	echo -e "class ${name} : public Game {" >> "src/${lowername}.hpp"
	echo -e "\tpublic:" >> "src/${lowername}.hpp"
	echo -e "\t\t${name}();" >> "src/${lowername}.hpp"
	echo -e "\t\t~${name}();" >> "src/${lowername}.hpp"
	echo -e "\tprotected:" >> "src/${lowername}.hpp"
	echo -e "\t\tvirtual void update(double time);" >> "src/${lowername}.hpp"
	echo -e "\tprivate:\n" >> "src/${lowername}.hpp"
	echo -e "};\n" >> "src/${lowername}.hpp"

	echo -e "#endif" >> "src/${lowername}.hpp"

	echo -e '#include "'"${lowername}.hpp"'"\n' > "src/${lowername}.cpp"
	echo -e '#include <iostream>\n\n' >> "src/${lowername}.cpp"

	echo -e "${name}::${name}() : Game("'"'"${name}"'") {' >> "src/${lowername}.cpp"
	echo -e "\t" >> "src/${lowername}.cpp"
	echo -e "}\n" >> "src/${lowername}.cpp"

	echo -e "${name}::~${name}() {" >> "src/${lowername}.cpp"
	echo -e "\t" >> "src/${lowername}.cpp"
	echo -e "}\n" >> "src/${lowername}.cpp"

	echo -e "void ${name}::update(double time) {" >> "src/${lowername}.cpp"
	echo -e "\trunning = running && this->window.getKey(GLFW_KEY_ESCAPE) == GLFW_RELEASE;\n\t" >> "src/${lowername}.cpp"
	echo -e "\tthis->window.clearScreen();" >> "src/${lowername}.cpp"
	echo -e "}\n" >> "src/${lowername}.cpp"

	echo -e "int main(int argc, char **argv) {" >> "src/${lowername}.cpp"
	echo -e "\ttry {" >> "src/${lowername}.cpp"
	echo -e "\t\t${name} game;" >> "src/${lowername}.cpp"
	echo -e "\t\tgame.run();" >> "src/${lowername}.cpp"
	echo -e "\t\treturn 0;" >> "src/${lowername}.cpp"
	echo -e "\t}" >> "src/${lowername}.cpp"
	echo -e "\tcatch(const std::exception &exc) {" >> "src/${lowername}.cpp"
	echo -e "\t\tstd::cerr << exc.what() << std::endl;" >> "src/${lowername}.cpp"
	echo -e "\t}" >> "src/${lowername}.cpp"
	echo -e "}" >> "src/${lowername}.cpp"
fi

