import java.io.IOException;
import java.net.UnknownHostException;
import java.util.List;

import netapp.manage.NaElement;
import netapp.manage.NaException;
import netapp.manage.NaServer;

public class ApiClient {

    public static void main(String[] args) {
        try {
            NaServer s = new NaServer("192.168.29.34");
            s.setServerType(NaServer.SERVER_TYPE_FILER);
            s.setTransportType(NaServer.TRANSPORT_TYPE_HTTPS);
            s.setPort(443);
            s.setStyle(NaServer.STYLE_LOGIN_PASSWORD);
            s.setAdminUser("vsadmin", "12!pass345");

/*
            NaElement input = new NaElement("snapdiff-iter-end");
            input.addNewChild("session-id", "26b05836-1efd-11e5-bcee-123478563412:5257baf0-ce3d-11e3-9dd3-c1eaa4272a35");
            NaElement xo = s.invokeElem(input);
            System.out.println(xo.toPrettyString(""));
*/

/*
            NaElement input = new NaElement("snapdiff-iter-start");
            input.addNewChild("base-snapshot", "multisnap_test1");
            input.addNewChild("diff-snapshot", "multisnap_test2");
            input.addNewChild("application-name", "actifio");
            input.addNewChild("application-type", "backup");
            input.addNewChild("volume", "vsrv1_vol2");
            input.addNewChild("max-diffs", "4096");


            NaElement xo = s.invokeElem(input);
            System.out.println(xo.toPrettyString(""));
*/

/*
            NaElement input = new NaElement("snapshot-multidelete");
            input.addNewChild("snapshot", "multisnap_test1");
            NaElement volumenames = new NaElement("volume-names");
            input.addChildElem(volumenames);
            volumenames.addNewChild("volume-name", "vsrv1_vol2");
            volumenames.addNewChild("volume-name", "vsrv1_vol3");

            NaElement xo = s.invokeElem(input);
            System.out.println(xo.toPrettyString(""));
*/

/*
            NaElement input = new NaElement("snapshot-multicreate");
            input.addNewChild("snapshot", "multisnap_test2");
            NaElement volumenames = new NaElement("volume-names");
            input.addChildElem(volumenames);
            volumenames.addNewChild("volume-name", "vsrv1_vol2");
            volumenames.addNewChild("volume-name", "vsrv1_vol3");

            NaElement xo = s.invokeElem(input);
            System.out.println(xo.toPrettyString(""));
*/

/*
            NaElement input = new NaElement("snapshot-multicreate-validate");
            input.addNewChild("snapshot", "multisnap_test1");
            NaElement volumenames = new NaElement("volume-names");
            input.addChildElem(volumenames);
            volumenames.addNewChild("volume-name", "vsrv1_vol2");
            volumenames.addNewChild("volume-name", "vsrv1_vol3");

            NaElement xo = s.invokeElem(input);
            System.out.println(xo.toPrettyString(""));
*/

/*
            NaElement input = new NaElement("snapshot-list-info");
            input.addNewChild("target-type", "volume");
            input.addNewChild("target-name", "vsrv1_vol2");

            NaElement xo = s.invokeElem(input);
            System.out.println(xo.toPrettyString(""));
*/

/*
            NaElement input = new NaElement("cifs-share-get-iter");

            NaElement xo = s.invokeElem(input);
            System.out.println(xo.toPrettyString(""));
*/
            NaElement input = new NaElement("nfs-exportfs-list-rules-2");

            NaElement xo = s.invokeElem(input);
            System.out.println(xo.toPrettyString(""));
/*
*/
/*
            NaElement input = new NaElement("export-rule-get-iter");

            NaElement xo = s.invokeElem(input);
            System.out.println(xo.toPrettyString(""));

*/
/*
            NaElement input = new NaElement("volume-get-iter");
            NaElement desired = new NaElement("desired-attributes");
            input.addChildElem(desired);
            NaElement volumeattr = new NaElement("volume-attributes");
            desired.addChildElem(volumeattr);
            NaElement exportattr = new NaElement("volume-export-attributes");
            volumeattr.addChildElem(exportattr);

            NaElement xo = s.invokeElem(input);
            System.out.println(xo.toPrettyString(""));
*/

/*
            // CIFS export
            NaElement api = new NaElement("cifs-share-create");
            api.addNewChild("path", "/vsrv1_vol1/.snapshot/testsnap1/a/aa");
            api.addNewChild("share-name","test_snapshare1");

            NaElement xo = s.invokeElem(input);
            System.out.println(xo.toPrettyString(""));
*/

/*
            // NFS export
            NaElement input = new NaElement("nfs-exportfs-modify-rule-2");
            input.addNewChild("persistent", "false");
            NaElement rule = new NaElement("rule");
            input.addChildElem(rule);
            NaElement exportRuleInfo = new NaElement("exports-rule-info-2");
            rule.addChildElem(exportRuleInfo);
            exportRuleInfo.addNewChild("pathname", "/vsrv1_vol1/b/vsrv1_vol3");
            NaElement rules = new NaElement("security-rules");
            exportRuleInfo.addChildElem(rules);
            NaElement ruleInfo = new NaElement("security-rule-info");
            rules.addChildElem(ruleInfo);

            // Add host to both root and read-only
            NaElement rootHost = new NaElement("root");
            ruleInfo.addChildElem(rootHost);
            NaElement readOnlyHost = new NaElement("read-only");
            ruleInfo.addChildElem(readOnlyHost);

            NaElement hostInfo = new NaElement("exports-hostname-info");
            rootHost.addChildElem(hostInfo);
            hostInfo.addNewChild("name", "192.168.29.37");

            NaElement xo = s.invokeElem(input);
            System.out.println(xo.toPrettyString(""));
*/


        } catch (NaException e) {
            handleException(e);
        } catch (UnknownHostException e) {
            handleException(e);
        } catch (IOException e) {
            handleException(e);
        }
    }

    private static void handleException(Exception e) {
        System.out.println(e.getMessage());
        e.printStackTrace();        
    }
}

