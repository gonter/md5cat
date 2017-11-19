
#include <stdio.h>

#include <gg/dpp.h>

#include <contrib/global.h>
#include <contrib/md5.h>
#include <contrib/md5gg.h>


int main (int argc, char *argv [])
{
  MD5_CTX md5_context;
  MD5Init (&md5_context);

  unsigned char digest [18];
  char digest_string [40];

  MD5Final (digest, &md5_context);
  MD5Digest2String ((char *) digest, digest_string);
    /* bin_to_hex ((char *) digest, digest_string, 16); */

  printf ("%-40s MD5 finger print %s\n", digest_string, "nix");
}
