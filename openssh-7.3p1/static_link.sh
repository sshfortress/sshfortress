
gcc -o sshd sshd.o auth-rhosts.o auth-passwd.o auth-rsa.o auth-rh-rsa.o audit.o audit-bsm.o audit-linux.o platform.o sshpty.o sshlogin.o servconf.o serverloop.o auth.o auth1.o auth2.o auth-options.o session.o auth-chall.o auth2-chall.o groupaccess.o auth-skey.o auth-bsdauth.o auth2-hostbased.o auth2-kbdint.o auth2-none.o auth2-passwd.o auth2-pubkey.o monitor_mm.o monitor.o monitor_wrap.o auth-krb5.o auth2-gss.o gss-serv.o gss-serv-krb5.o loginrec.o auth-pam.o auth-shadow.o auth-sia.o md5crypt.o sftp-server.o sftp-common.o sandbox-null.o sandbox-rlimit.o sandbox-systrace.o sandbox-darwin.o sandbox-seccomp-filter.o sandbox-capsicum.o sandbox-pledge.o sandbox-solaris.o cache.o sshfs.o -L. -Lopenbsd-compat/ -L/usr/local/ssl/lib  -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -fstack-protector-strong -pie -lssh -lopenbsd-compat  /usr/local/ssl/lib/libcrypto.a  -ldl -lutil -lz -lnsl  -lcrypt -lresolv /usr/local/mysql/lib/libmysqlclient.a  -lpthread -lm -lrt -ldl  -lstdc++ static_lib/libfuse3.a static_lib/glib-2.0.a static_lib/libcharset.a





ar cr glib-2.0.a deprecated_gallocator.c.o gbitlock.c.o  gerror.c.o  glib-private.c.o  gpoll.c.o  gscanner.c.o  gthread.c.o  guniprop.c.o  gversion.c.o deprecated_gcache.c.o \
	gbookmarkfile.c.o  gfileutils.c.o  glib-unix.c.o  gprimes.c.o gsequence.c.o  gthreadpool.c.o  gurifuncs.c.o     gwakeup.c.o \
	deprecated_gcompletion.c.o         gbytes.c.o         ggettext.c.o    glist.c.o         gprintf.c.o   gshell.c.o        gthread-posix.c.o  gutf8.c.o             \
	deprecated_grel.c.o                gcharset.c.o       ghash.c.o       gmain.c.o         gqsort.c.o    gslice.c.o        gtimer.c.o         gutils.c.o \
	deprecated_gthread-deprecated.c.o  gchecksum.c.o      ghmac.c.o       gmappedfile.c.o   gquark.c.o     gslist.c.o        gtimezone.c.o      guuid.c.o \
	garcbox.c.o                        gconvert.c.o       ghook.c.o       gmarkup.c.o       gqueue.c.o    gspawn.c.o        gtranslit.c.o      gvariant.c.o \
	garray.c.o                         gdataset.c.o       ghostutils.c.o  gmem.c.o          grand.c.o     gstdio.c.o        gtrashstack.c.o    gvariant-core.c.o \
	gasyncqueue.c.o                    gdate.c.o          giochannel.c.o  gmessages.c.o     grcbox.c.o    gstrfuncs.c.o     gtree.c.o          gvariant-parser.c.o \
	gatomic.c.o                        gdatetime.c.o      giounix.c.o     gnode.c.o         grefcount.c.o gstringchunk.c.o  gunibreak.c.o      gvariant-serialiser.c.o \
	gbacktrace.c.o                     gdir.c.o           gkeyfile.c.o    goption.c.o       grefstring.c.o  gstring.c.o       gunicollate.c.o    gvarianttype.c.o \
	gbase64.c.o                        genviron.c.o       glib-init.c.o   gpattern.c.o      gregex.c.o      gtestutils.c.o    gunidecomp.c.o     gvarianttypeinfo.c.o


ar cr libfuse3.a buffer.c.o cuse_lowlevel.c.o fuse.c.o fuse_log.c.o fuse_loop.c.o fuse_loop_mt.c.o fuse_lowlevel.c.o fuse_opt.c.o fuse_signals.c.o helper.c.o modules_iconv.c.o modules_subdir.c.o mount.c.o mount_util.c.o libfuse3.so.3.9.2.symbols
