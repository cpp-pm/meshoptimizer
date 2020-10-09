// This file is part of gltfpack; see gltfpack.h for version/license details
#include "../gltf/gltfpack.h"
#include "../src/meshoptimizer.h"

#include <cstring>

int main(int argc, char** argv)
{
	meshopt_encodeIndexVersion(1);

	Settings settings = {};
	settings.quantize = true;
	settings.pos_bits = 14;
	settings.tex_bits = 12;
	settings.nrm_bits = 8;
	settings.trn_bits = 16;
	settings.rot_bits = 12;
	settings.scl_bits = 16;
	settings.anim_freq = 30;
	settings.simplify_threshold = 1.f;
	settings.texture_quality = 8;
	settings.texture_scale = 1.f;

	const char* input = 0;
	const char* output = 0;
	const char* report = 0;
	bool help = false;
	bool test = false;

	std::vector<const char*> testinputs;

	for (int i = 1; i < argc; ++i)
	{
		const char* arg = argv[i];

		if (strcmp(arg, "-vp") == 0 && i + 1 < argc && isdigit(argv[i + 1][0]))
		{
			settings.pos_bits = atoi(argv[++i]);
		}
		else if (strcmp(arg, "-vt") == 0 && i + 1 < argc && isdigit(argv[i + 1][0]))
		{
			settings.tex_bits = atoi(argv[++i]);
		}
		else if (strcmp(arg, "-vn") == 0 && i + 1 < argc && isdigit(argv[i + 1][0]))
		{
			settings.nrm_bits = atoi(argv[++i]);
		}
		else if (strcmp(arg, "-at") == 0 && i + 1 < argc && isdigit(argv[i + 1][0]))
		{
			settings.trn_bits = atoi(argv[++i]);
		}
		else if (strcmp(arg, "-ar") == 0 && i + 1 < argc && isdigit(argv[i + 1][0]))
		{
			settings.rot_bits = atoi(argv[++i]);
		}
		else if (strcmp(arg, "-as") == 0 && i + 1 < argc && isdigit(argv[i + 1][0]))
		{
			settings.scl_bits = atoi(argv[++i]);
		}
		else if (strcmp(arg, "-af") == 0 && i + 1 < argc && isdigit(argv[i + 1][0]))
		{
			settings.anim_freq = atoi(argv[++i]);
		}
		else if (strcmp(arg, "-ac") == 0)
		{
			settings.anim_const = true;
		}
		else if (strcmp(arg, "-kn") == 0)
		{
			settings.keep_nodes = true;
		}
		else if (strcmp(arg, "-km") == 0)
		{
			settings.keep_materials = true;
		}
		else if (strcmp(arg, "-ke") == 0)
		{
			settings.keep_extras = true;
		}
		else if (strcmp(arg, "-mm") == 0)
		{
			settings.mesh_merge = true;
		}
		else if (strcmp(arg, "-mi") == 0)
		{
			settings.mesh_instancing = true;
		}
		else if (strcmp(arg, "-si") == 0 && i + 1 < argc && isdigit(argv[i + 1][0]))
		{
			settings.simplify_threshold = float(atof(argv[++i]));
		}
		else if (strcmp(arg, "-sa") == 0)
		{
			settings.simplify_aggressive = true;
		}
#ifndef NDEBUG
		else if (strcmp(arg, "-sd") == 0 && i + 1 < argc && isdigit(argv[i + 1][0]))
		{
			settings.simplify_debug = float(atof(argv[++i]));
		}
		else if (strcmp(arg, "-md") == 0 && i + 1 < argc && isdigit(argv[i + 1][0]))
		{
			settings.meshlet_debug = atoi(argv[++i]);
		}
#endif
		else if (strcmp(arg, "-tu") == 0)
		{
			settings.texture_ktx2 = true;
			settings.texture_uastc = true;
		}
		else if (strcmp(arg, "-tc") == 0)
		{
			settings.texture_ktx2 = true;
		}
		else if (strcmp(arg, "-tq") == 0 && i + 1 < argc && isdigit(argv[i + 1][0]))
		{
			settings.texture_quality = atoi(argv[++i]);
		}
		else if (strcmp(arg, "-ts") == 0 && i + 1 < argc && isdigit(argv[i + 1][0]))
		{
			settings.texture_scale = float(atof(argv[++i]));
		}
		else if (strcmp(arg, "-tp") == 0)
		{
			settings.texture_pow2 = true;
		}
		else if (strcmp(arg, "-noq") == 0)
		{
			settings.quantize = false;
		}
		else if (strcmp(arg, "-i") == 0 && i + 1 < argc && !input)
		{
			input = argv[++i];
		}
		else if (strcmp(arg, "-o") == 0 && i + 1 < argc && !output)
		{
			output = argv[++i];
		}
		else if (strcmp(arg, "-r") == 0 && i + 1 < argc && !report)
		{
			report = argv[++i];
		}
		else if (strcmp(arg, "-c") == 0)
		{
			settings.compress = true;
		}
		else if (strcmp(arg, "-cc") == 0)
		{
			settings.compress = true;
			settings.compressmore = true;
		}
		else if (strcmp(arg, "-cf") == 0)
		{
			settings.compress = true;
			settings.fallback = true;
		}
		else if (strcmp(arg, "-v") == 0)
		{
			settings.verbose = 1;
		}
		else if (strcmp(arg, "-vv") == 0)
		{
			settings.verbose = 2;
		}
		else if (strcmp(arg, "-h") == 0)
		{
			help = true;
		}
		else if (strcmp(arg, "-test") == 0)
		{
			test = true;
		}
		else if (arg[0] == '-')
		{
			fprintf(stderr, "Unrecognized option %s\n", arg);
			return 1;
		}
		else if (test)
		{
			testinputs.push_back(arg);
		}
		else
		{
			fprintf(stderr, "Expected option, got %s instead\n", arg);
			return 1;
		}
	}

	// shortcut for gltfpack -v
	if (settings.verbose && argc == 2)
	{
		printf("gltfpack %s\n", getVersion().c_str());
		return 0;
	}

	if (test)
	{
		for (size_t i = 0; i < testinputs.size(); ++i)
		{
			const char* path = testinputs[i];

			printf("%s\n", path);
			gltfpack(path, NULL, NULL, settings);
		}

		return 0;
	}

	if (!input || !output || help)
	{
		fprintf(stderr, "gltfpack %s\n", getVersion().c_str());
		fprintf(stderr, "Usage: gltfpack [options] -i input -o output\n");

		if (help)
		{
			fprintf(stderr, "\nBasics:\n");
			fprintf(stderr, "\t-i file: input file to process, .obj/.gltf/.glb\n");
			fprintf(stderr, "\t-o file: output file path, .gltf/.glb\n");
			fprintf(stderr, "\t-c: produce compressed gltf/glb files (-cc for higher compression ratio)\n");
			fprintf(stderr, "\nTextures:\n");
			fprintf(stderr, "\t-tc: convert all textures to KTX2 with BasisU supercompression (using basisu/toktx executable)\n");
			fprintf(stderr, "\t-tu: use UASTC when encoding textures (much higher quality and much larger size)\n");
			fprintf(stderr, "\t-tq N: set texture encoding quality (default: 8; N should be between 1 and 10\n");
			fprintf(stderr, "\t-ts R: scale texture dimensions by the ratio R (default: 1; R should be between 0 and 1)\n");
			fprintf(stderr, "\t-tp: resize textures to nearest power of 2 to conform to WebGL1 restrictions\n");
			fprintf(stderr, "\nSimplification:\n");
			fprintf(stderr, "\t-si R: simplify meshes to achieve the ratio R (default: 1; R should be between 0 and 1)\n");
			fprintf(stderr, "\t-sa: aggressively simplify to the target ratio disregarding quality\n");
			fprintf(stderr, "\nVertices:\n");
			fprintf(stderr, "\t-vp N: use N-bit quantization for positions (default: 14; N should be between 1 and 16)\n");
			fprintf(stderr, "\t-vt N: use N-bit quantization for texture coordinates (default: 12; N should be between 1 and 16)\n");
			fprintf(stderr, "\t-vn N: use N-bit quantization for normals and tangents (default: 8; N should be between 1 and 16)\n");
			fprintf(stderr, "\nAnimations:\n");
			fprintf(stderr, "\t-at N: use N-bit quantization for translations (default: 16; N should be between 1 and 24)\n");
			fprintf(stderr, "\t-ar N: use N-bit quantization for rotations (default: 12; N should be between 4 and 16)\n");
			fprintf(stderr, "\t-as N: use N-bit quantization for scale (default: 16; N should be between 1 and 24)\n");
			fprintf(stderr, "\t-af N: resample animations at N Hz (default: 30)\n");
			fprintf(stderr, "\t-ac: keep constant animation tracks even if they don't modify the node transform\n");
			fprintf(stderr, "\nScene:\n");
			fprintf(stderr, "\t-kn: keep named nodes and meshes attached to named nodes so that named nodes can be transformed externally\n");
			fprintf(stderr, "\t-km: keep named materials and disable named material merging\n");
			fprintf(stderr, "\t-ke: keep extras data\n");
			fprintf(stderr, "\t-mm: merge instances of the same mesh together when possible\n");
			fprintf(stderr, "\t-mi: use EXT_mesh_gpu_instancing when serializing multiple mesh instances\n");
			fprintf(stderr, "\nMiscellaneous:\n");
			fprintf(stderr, "\t-cf: produce compressed gltf/glb files with fallback for loaders that don't support compression\n");
			fprintf(stderr, "\t-noq: disable quantization; produces much larger glTF files with no extensions\n");
			fprintf(stderr, "\t-v: verbose output (print version when used without other options)\n");
			fprintf(stderr, "\t-r file: output a JSON report to file\n");
			fprintf(stderr, "\t-h: display this help and exit\n");
		}
		else
		{
			fprintf(stderr, "\nBasics:\n");
			fprintf(stderr, "\t-i file: input file to process, .obj/.gltf/.glb\n");
			fprintf(stderr, "\t-o file: output file path, .gltf/.glb\n");
			fprintf(stderr, "\t-c: produce compressed gltf/glb files (-cc for higher compression ratio)\n");
			fprintf(stderr, "\t-tc: convert all textures to KTX2 with BasisU supercompression (using basisu/toktx executable)\n");
			fprintf(stderr, "\t-si R: simplify meshes to achieve the ratio R (default: 1; R should be between 0 and 1)\n");
			fprintf(stderr, "\nRun gltfpack -h to display a full list of options\n");
		}

		return 1;
	}

	if (settings.texture_scale < 1 && !settings.texture_ktx2)
	{
		fprintf(stderr, "Option -ts is only supported when -tc is set as well\n");
		return 1;
	}

	if (settings.texture_pow2 && !settings.texture_ktx2)
	{
		fprintf(stderr, "Option -tp is only supported when -tc is set as well\n");
		return 1;
	}

	return gltfpack(input, output, report, settings);
}
