FROM gcc:7.3


RUN apt-get update \
    && apt-get -y install git  bzr lib32z1 lib32ncurses5 nodejs vim

RUN ln -s `which nodejs` /usr/bin/node

WORKDIR /home/dev

RUN wget -q https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2018q2/gcc-arm-none-eabi-7-2018-q2-update-linux.tar.bz2 \
    && tar xf gcc-arm-none-eabi-7-2018-q2-update-linux.tar.bz2 \
    && rm gcc-arm-none-eabi-7-2018-q2-update-linux.tar.bz2


ENV PATH $PATH:/home/dev/gcc-arm-none-eabi-7-2018-q2-update/bin

COPY . .

RUN cd system/scripts \
    && ./build_full.sh

CMD [bash]







