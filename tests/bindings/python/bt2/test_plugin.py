#
# Copyright (C) 2019 EfficiOS Inc.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; only version 2
# of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#

import unittest
import bt2
import bt2.plugin
import os


_TEST_PLUGIN_PLUGINS_PATH = os.environ['BT_PLUGINS_PATH']


class PluginSetTestCase(unittest.TestCase):
    def test_create(self):
        pset = bt2.find_plugins(_TEST_PLUGIN_PLUGINS_PATH)
        self.assertTrue(len(pset) >= 3)

    def test_getitem(self):
        pset = bt2.find_plugins(_TEST_PLUGIN_PLUGINS_PATH)
        self.assertTrue(pset[0].path.startswith(_TEST_PLUGIN_PLUGINS_PATH))

    def test_iter(self):
        pset = bt2.find_plugins(_TEST_PLUGIN_PLUGINS_PATH)
        names = set()

        for plugin in pset:
            names.add(plugin.name)

        self.assertTrue('ctf' in names)
        self.assertTrue('utils' in names)
        self.assertTrue('text' in names)


class FindPluginsTestCase(unittest.TestCase):
    def test_find_nonexistent_dir(self):
        with self.assertRaises(bt2.Error):
            bt2.find_plugins('/this/does/not/exist/246703df-cb85-46d5-8406-5e8dc4a88b41')

    def test_find_none_existing_dir(self):
        plugins = bt2.find_plugins(_TEST_PLUGIN_PLUGINS_PATH, recurse=False)
        self.assertIsNone(plugins)

    def test_find_dir(self):
        pset = bt2.find_plugins(_TEST_PLUGIN_PLUGINS_PATH)
        self.assertTrue(len(pset) >= 3)

    def test_find_file(self):
        path = os.path.join(_TEST_PLUGIN_PLUGINS_PATH, 'utils', '.libs', 'babeltrace-plugin-utils.so')
        pset = bt2.find_plugins(path)
        self.assertTrue(len(pset) == 1)


class FindPluginTestCase(unittest.TestCase):
    def test_find_none(self):
        plugin = bt2.find_plugin('this-does-not-exist-246703df-cb85-46d5-8406-5e8dc4a88b41')
        self.assertIsNone(plugin)

    def test_find_existing(self):
        plugin = bt2.find_plugin('ctf')
        self.assertIsInstance(plugin, bt2.plugin._Plugin)


class PluginTestCase(unittest.TestCase):
    def setUp(self):
        self._plugin = bt2.find_plugin('ctf')

    def tearDown(self):
        del self._plugin

    def test_name(self):
        self.assertEqual(self._plugin.name, 'ctf')

    def test_path(self):
        plugin_path = os.path.normcase(self._plugin.path)
        plugin_path_env = os.path.normcase(_TEST_PLUGIN_PLUGINS_PATH)
        self.assertTrue(plugin_path.startswith(plugin_path_env))

    def test_author(self):
        self.assertTrue('Philippe Proulx' in self._plugin.author)

    def test_license(self):
        self.assertTrue('MIT' in self._plugin.license)

    def test_description(self):
        self.assertTrue('CTF source and sink support' in self._plugin.description)

    def test_version(self):
        self.assertIsNone(self._plugin.version)

    def test_source_comp_classes_len(self):
        self.assertEqual(len(self._plugin.source_component_classes), 2)

    def test_source_comp_classes_getitem(self):
        self.assertEqual(self._plugin.source_component_classes['fs'].name, 'fs')

    def test_source_comp_classes_getitem_invalid(self):
        with self.assertRaises(KeyError):
            self._plugin.source_component_classes['lol']

    def test_source_comp_classes_iter(self):
        plugins = {}

        for cc_name, cc in self._plugin.source_component_classes.items():
            plugins[cc_name] = cc

        self.assertTrue('fs' in plugins)
        self.assertTrue('lttng-live' in plugins)
        self.assertEqual(plugins['fs'].name, 'fs')
        self.assertEqual(plugins['lttng-live'].name, 'lttng-live')

    def test_filter_comp_classes_len(self):
        plugin = bt2.find_plugin('utils')
        self.assertEqual(len(plugin.filter_component_classes), 2)

    def test_sink_comp_classes_len(self):
        self.assertEqual(len(self._plugin.sink_component_classes), 1)
