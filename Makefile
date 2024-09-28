cc = clang
cflags = -Wall
lflags = -lm

gen_pyscripts = $(wildcard utils/gen/*.py)
gen_cscripts = $(wildcard utils/gen/*.c)
gen_pysources = $(patsubst utils/gen/%.py,target/gen/%.c,$(gen_pyscripts))
gen_csources = $(patsubst utils/gen/%.c,target/gen/%.c,$(gen_cscripts))
gen_sources = $(gen_pysources) $(gen_csources)
gen_objs = $(patsubst target/gen/%.c,target/obj/gen/%.o,$(gen_sources))

sources = $(wildcard src/*.c src/*/*.c)
headers = $(wildcard src/*.h src/*/*.h)
objs = $(patsubst src/%.c,target/obj/%.o,$(sources))

dump_sources = $(wildcard src/dump/*.c)
dump_bins = $(patsubst src/dump/%.c,target/bin/dump/%,$(dump_sources))
dump_imgs = $(patsubst src/dump/%.c,target/img/%.png,$(dump_sources))

render_sources = $(wildcard src/render/*.c)
render_bins = $(patsubst src/render/%.c,target/bin/render/%,$(render_sources))
render_imgs = $(patsubst src/render/%.c,target/img/%.png,$(render_sources))

test_sources = $(wildcard src/test/*.c)
test_bins = $(patsubst src/test/%.c,target/bin/test/%,$(test_sources))

main_sources = $(wildcard src/*.c)
main_objs = $(patsubst src/%.c,target/obj/%.o,$(main_sources))

bins = $(test_bins) $(dump_bins) $(render_bins)
imgs = $(dump_imgs) $(render_imgs)

.PHONY: clean all
all: $(bins)

clean:
	rm -rf target

dirs = target/gen target/obj target/bin target/data target/img target/src/gen target/obj/gen target/obj/dump target/render target/bin/dump target/bin/render

$(dirs): %:
	mkdir -p $*

$(gen_pysources): target/gen/%.c: utils/gen/%.py
	mkdir -p target/gen
	python3 utils/gen/$*.py > target/gen/$*.c

$(gen_csources): target/gen/%.c: utils/gen/%.c
	mkdir -p target/gen
	tcc -run utils/gen/$*.c > target/gen/$*.c

$(gen_objs): target/obj/gen/%.o: target/gen/%.c
	mkdir -p target/obj/gen
	$(cc) -c $(cflags) -o target/obj/gen/$*.o target/gen/$*.c

$(objs): target/obj/%.o: src/%.c $(gen_objs) $(sources) $(headers)
	mkdir -p $(@D)
	$(cc) $(cflags) -c -o target/obj/$*.o src/$*.c

$(bins): target/bin/%: target/obj/%.o $(main_objs)
	mkdir -p $(@D)
	$(cc) $(lflags) -o target/bin/$* $(main_objs) $(gen_objs) target/obj/$*.o

$(dump_imgs): target/img/%.png: target/bin/dump/%
	mkdir -p $(@D)
	target/bin/dump/$* | python3 utils/encode/csv_to_plot.py target/img/$*.png

$(render_imgs): target/img/%.png: target/bin/render/%
	mkdir -p $(@D)
	target/bin/render/$* | python3 utils/encode/matrix_to_img.py target/img/$*.png
