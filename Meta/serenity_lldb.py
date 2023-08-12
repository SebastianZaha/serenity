# Copyright (c) 2023, Sebastian Zaha <sebastian.zaha@gmail.com>
#
# SPDX-License-Identifier: BSD-2-Clause

import lldb

# https://lldb.llvm.org/use/variable.html#synthetic-children

def uint(sbval, member):
    return sbval.GetChildMemberWithName(member).GetValueAsUnsigned(0)

def log(string):
    logger = lldb.formatters.Logger.Logger()
    logger >> string

class AKVector_SyntheticChildrenProvider: 
    def __init__(self, val, dict):
        self.val = val
        self.element_type = self.val.GetType().GetTemplateArgumentType(0)
        self.element_size = self.element_type.GetByteSize()

    def num_children(self):
        return uint(self.val, "m_size")

    def get_child_index(self, name):
        log("get_child_index '" + name + "'")
        try:
            return int(name.lstrip("[").rstrip("]"))
        except:
            return -1

    def get_child_at_index(self, index):
        log("get_child_at_index '" + str(index) + "'")

        if index < 0:
            return None
        if index >= self.num_children():
            return None
        try:
            offset = index * self.element_size
            return self.data.CreateChildAtOffset("[" + str(index) + "]", offset, self.element_type)
        except:
            return None

    def update(self):
        outline = self.val.GetChildMemberWithName("m_outline_buffer")
        self.data = outline

        #data =  or self.val.GetChildMemberWithName("m_inline_buffer_storage")

def __lldb_init_module(debugger, unused):
    debugger.HandleCommand('type synthetic add -x "AK::Vector<" -l serenity_lldb.AKVector_SyntheticChildrenProvider')
