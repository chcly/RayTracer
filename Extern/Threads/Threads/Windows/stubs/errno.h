#pragma once
extern int errno;


typedef unsigned long long __syscall_slong_t;


struct timespec
{
    __syscall_slong_t a, b;
};


enum errnoCode
{
    EBUSY,
    EINVAL,
    EAGAIN,
    ENOMEM,
    EPERM
};