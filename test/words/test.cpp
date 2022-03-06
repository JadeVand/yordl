
#include <urand.h>
#include <iostream>
#include <string.h>
int uuid_str_valid(const char *uuid)
{
    int i, valid;

    if (uuid == NULL)
        return 0;

    for (i = 0, valid = 1; uuid[i] && valid; i++) {
        switch (i) {
        case 8: case 13: case 18: case 23:
            valid = (uuid[i] == '-');
            break;
        default:
            valid = isxdigit(uuid[i]);
            break;
        }
    }

    if (i != 36 || !valid)
        return 0;

    return 1;
}
int uuid_str_to_bin(char *uuid_str, unsigned char *uuid_bin, int str_format)
{
    uint16_t tmp16;
    uint32_t tmp32;
    uint64_t tmp64;

    if (!uuid_str_valid(uuid_str))
        return -EINVAL;

    if (str_format == UUID_STR_FORMAT_STD) {
        tmp32 = cpu_to_be32(strtoul(uuid_str, NULL, 16));
        memcpy(uuid_bin, &tmp32, 4);

        tmp16 = cpu_to_be16(strtoul(uuid_str + 9, NULL, 16));
        memcpy(uuid_bin + 4, &tmp16, 2);

        tmp16 = cpu_to_be16(strtoul(uuid_str + 14, NULL, 16));
        memcpy(uuid_bin + 6, &tmp16, 2);
    } else {
        tmp32 = cpu_to_le32(strtoul(uuid_str, NULL, 16));
        memcpy(uuid_bin, &tmp32, 4);

        tmp16 = cpu_to_le16(strtoul(uuid_str + 9, NULL, 16));
        memcpy(uuid_bin + 4, &tmp16, 2);

        tmp16 = cpu_to_le16(strtoul(uuid_str + 14, NULL, 16));
        memcpy(uuid_bin + 6, &tmp16, 2);
    }

    tmp16 = cpu_to_be16(strtoul(uuid_str + 19, NULL, 16));
    memcpy(uuid_bin + 8, &tmp16, 2);

    tmp64 = cpu_to_be64(strtoul(uuid_str + 24, NULL, 16));
    memcpy(uuid_bin + 10, (char *)&tmp64 + 2, 6);

    return 0;
}
int main(){
    URand ur;
    char u[37] ={0};
    uint8_t b[18] = {0};
    ur.uuid4_generate(u);
    printf("%s\t%d\n",u,uuid_str_valid(u));
    if(uuid_str_to_bin(u,b,0)){
        printf("success\n");
    }
}
