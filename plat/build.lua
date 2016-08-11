include("mach/proto/as/build.lua")
include("mach/proto/ncg/build.lua")

definerule("ackfile",
	{
		srcs = { type="targets" },
		deps = { type="targets", default={} },
	},
	function (e)
		local plat = e.vars.plat

		return cfile {
			name = e.name,
			srcs = e.srcs,
			deps = {
				"lang/cem/cpp.ansi+pkg",
				"plat/"..plat.."+tools",
				"util/ack+pkg",
				"util/misc+pkg",
				e.deps
			},
			commands = {
				"ACKDIR=$(INSDIR) $(INSDIR)/bin/ack -m%{plat} -c -o %{outs} %{ins} %{hdrpaths} %{ackcflags}"
			}
		}
	end
)

definerule("acklibrary",
	{
		srcs = { type="targets", default={} },
		hdrs = { type="targets", default={} },
		deps = { type="targets", default={} },
	},
	function (e)
		return clibrary {
			name = e.name,
			srcs = e.srcs,
			hdrs = e.hdrs,
			deps = {
				"util/arch+pkg",
				e.deps
			},
			_cfile = ackfile,
			commands = {
				"rm -f %{outs[1]}",
				"ACKDIR=$(INSDIR) $(INSDIR)/bin/aal qc %{outs[1]} %{ins}"
			}
		}
	end
)

definerule("build_plat_tools",
	{
		arch = { type="string" },
		plat = { type="string" },
	},
	function(e)
		local descr = "plat/"..e.plat.."/descr"

		local as = build_as {
			name = "as",
			arch = e.arch
		}

		local ncg = build_ncg {
			name = "ncg",
			arch = e.arch,
		}

		return installable {
			name = e.name,
			map = {
				["$(PLATDEP)/"..e.plat.."/as"] = as,
				["$(PLATDEP)/"..e.plat.."/ncg"] = ncg,
				["$(PLATIND)/descr/"..e.plat] = descr,
				"util/opt+pkg",
			}
		}
	end
)

definerule("build_plat_libs",
	{
		arch = { type="string" },
		plat = { type="string" },
	},
	function(e)
		return installable {
			name = e.name,
			map = {
				["$(PLATIND)/"..e.plat.."/libend.a"] = "mach/"..e.arch.."/libend+lib_"..e.plat,
				["$(PLATIND)/"..e.plat.."/libem.a"] = "mach/"..e.arch.."/libem+lib_"..e.plat,
				["$(PLATIND)/"..e.plat.."/libsys.a"] = "mach/"..e.arch.."/libsys+lib_"..e.plat,
				"lang/cem/libcc.ansi+pkg_"..e.plat,
			}
		}
	end
)

