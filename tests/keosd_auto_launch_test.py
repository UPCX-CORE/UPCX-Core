#!/usr/bin/env python3

# This script tests that clupcx launches kupcxd automatically when kupcxd is not
# running yet.

import subprocess


def run_clupcx_wallet_command(command: str, no_auto_kupcxd: bool):
    """Run the given clupcx command and return subprocess.CompletedProcess."""
    args = ['./programs/clupcx/clupcx']

    if no_auto_kupcxd:
        args.append('--no-auto-kupcxd')

    args += 'wallet', command

    return subprocess.run(args,
                          check=False,
                          stdout=subprocess.DEVNULL,
                          stderr=subprocess.PIPE)


def stop_kupcxd():
    """Stop the default kupcxd instance."""
    run_clupcx_wallet_command('stop', no_auto_kupcxd=True)


def check_clupcx_stderr(stderr: bytes, expected_match: bytes):
    if expected_match not in stderr:
        raise RuntimeError("'{}' not found in {}'".format(
            expected_match.decode(), stderr.decode()))


def kupcxd_auto_launch_test():
    """Test that kupcx auto-launching works but can be optionally inhibited."""
    stop_kupcxd()

    # Make sure that when '--no-auto-kupcxd' is given, kupcxd is not started by
    # clupcx.
    completed_process = run_clupcx_wallet_command('list', no_auto_kupcxd=True)
    assert completed_process.returncode != 0
    check_clupcx_stderr(completed_process.stderr, b'Failed to connect to kupcxd')

    # Verify that kupcxd auto-launching works.
    completed_process = run_clupcx_wallet_command('list', no_auto_kupcxd=False)
    if completed_process.returncode != 0:
        raise RuntimeError("Expected that kupcxd would be started, "
                           "but got an error instead: {}".format(
                               completed_process.stderr.decode()))
    check_clupcx_stderr(completed_process.stderr, b'launched')


try:
    kupcxd_auto_launch_test()
finally:
    stop_kupcxd()
