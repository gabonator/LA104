FROM gcc:7.3


RUN apt-get update \
    && apt-get -y install git bzr lib32z1 lib32ncurses5 nodejs vim zip imagemagick

RUN ln -s `which nodejs` /usr/bin/node

WORKDIR /home/dev

RUN wget -q https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 \
    && tar xf gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 \
    && rm gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2

ENV PATH $PATH:/home/dev/gcc-arm-none-eabi-9-2020-q2-update/bin

COPY . .

RUN cd system/scripts \
    && ./build.sh verbose \
    && cd ../.. \
    && mkdir output \
    && cp system/build/firmware/*.* output/ \
    && cp system/build/*.* output/

CMD [bash]







